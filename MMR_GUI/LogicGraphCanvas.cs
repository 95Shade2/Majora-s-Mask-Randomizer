using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    public sealed class GraphInventoryEventArgs : EventArgs
    {
        public string Placement { get; private set; }
        public bool FullRefresh { get; private set; }

        public GraphInventoryEventArgs(string placement)
        {
            Placement = placement ?? "";
            FullRefresh = false;
        }

        public GraphInventoryEventArgs(bool fullRefresh)
        {
            Placement = "";
            FullRefresh = fullRefresh;
        }
    }

    public sealed class GraphNodeEventArgs : EventArgs
    {
        public GraphNode Node { get; private set; }

        public GraphNodeEventArgs(GraphNode node)
        {
            Node = node;
        }
    }

    public enum GraphNodeKind
    {
        StartItem,
        Location
    }

    public sealed class GraphNode
    {
        public string Key;
        public string DisplayName;
        public GraphNodeKind Kind;
        public int Column;
        public float X;
        public float Y;
        public float Width;
        public float Height;
        public bool Expanded;
        public bool Revealed;
        public bool IsRootColumn;
        public RectangleF Bounds;
    }

    internal sealed class GraphEdge
    {
        public GraphNode From;
        public GraphNode To;
    }

    [ToolboxItem(false)]
    public sealed class GraphCanvas : Panel
    {
        public event EventHandler<GraphNodeEventArgs> NodeSelected;
        public event EventHandler<GraphNodeEventArgs> NodeHovered;
        public event EventHandler ProgressChanged;
        public event EventHandler<GraphInventoryEventArgs> InventoryChanged;

        private readonly List<GraphNode> _nodes = new List<GraphNode>();
        private readonly List<GraphEdge> _edges = new List<GraphEdge>();
        private readonly Dictionary<string, GraphNode> _nodeLookup =
            new Dictionary<string, GraphNode>();

        private LogicUsefulnessResult _logicData;
        private Dictionary<string, string> _placements;
        private readonly HashSet<string> _rootColumnLocations = new HashSet<string>();
        private readonly HashSet<string> _startingInventory = new HashSet<string>
        {
            "Deku Mask",
            "Song of Healing"
        };

        private GraphNode _pendingNode;
        private GraphNode _hoveredNode;
        private GraphNode _searchSelectedNode;
        private bool _shiftHoverMode;
        private HashSet<GraphNode> _highlightedDescendants = new HashSet<GraphNode>();
        private bool _panning;
        private bool _minimapPanning;
        private Point _mouseDownPoint;
        private PointF _panOffsetStart;
        private PointF _viewOffset = PointF.Empty;

        private const float MinNodeWidth = 156f;
        private const float MinNodeHeight = 34f;
        private const float NodePaddingX = 16f;
        private const float NodePaddingY = 12f;
        private const float ColumnGap = 220f;
        private const float RowGap = 14f;
        private const float GraphMargin = 32f;
        private const int DragThreshold = 4;
        private const float MinimapWidth = 160f;
        private const float MinimapHeight = 110f;
        private const float MinimapMargin = 10f;
        private static readonly Color SearchSelectedBorderColor = Color.FromArgb(255, 193, 37);
        private static readonly Color HoverIncomingParentBorderColor = Color.FromArgb(240, 220, 255);
        private static readonly Color HoverDescendantBorderColor = Color.FromArgb(120, 220, 210);

        private float _uniformNodeWidth = MinNodeWidth;
        private float _uniformNodeHeight = MinNodeHeight;

        private readonly HashSet<string> _locationsNotOnGraph = new HashSet<string>(StringComparer.Ordinal);
        private readonly HashSet<string> _edgeKeys = new HashSet<string>(StringComparer.Ordinal);
        private readonly HashSet<string> _nodesWithIncomingEdge = new HashSet<string>(StringComparer.Ordinal);
        private readonly Dictionary<string, List<GraphNode>> _incomingParentsByChildKey =
            new Dictionary<string, List<GraphNode>>(StringComparer.Ordinal);

        private HashSet<string> _cachedAcquiredItems;
        private Dictionary<string, int> _cachedGrantCounts;
        private bool _hasAcquiredCache;

        private GraphicsPath _roundedNodePath;
        private float _roundedNodePathWidth;
        private float _roundedNodePathHeight;

        private sealed class AcquiredSnapshot
        {
            public HashSet<string> Acquired;
            public Dictionary<string, int> GrantCounts;
        }

        public List<string> FindMatchingNodes(string query)
        {
            List<string> matches = new List<string>();

            if (string.IsNullOrWhiteSpace(query))
            {
                return matches;
            }

            string lowered = query.Trim().ToLowerInvariant();

            foreach (GraphNode node in _nodes.OrderBy(n => n.DisplayName))
            {
                if (!NodeMatchesSearch(node, lowered))
                {
                    continue;
                }

                matches.Add(node.DisplayName);
            }

            return matches;
        }

        public bool FocusNode(string displayName)
        {
            if (displayName == "")
            {
                return false;
            }

            GraphNode node = _nodes.FirstOrDefault(
                n => string.Equals(n.DisplayName, displayName, StringComparison.OrdinalIgnoreCase));

            if (node == null)
            {
                return false;
            }

            float centerX = node.Bounds.X + node.Bounds.Width / 2f;
            float centerY = node.Bounds.Y + node.Bounds.Height / 2f;

            _viewOffset = new PointF(
                ClientSize.Width / 2f - centerX,
                ClientSize.Height / 2f - centerY);

            _searchSelectedNode = node;

            if (NodeSelected != null)
            {
                NodeSelected(this, new GraphNodeEventArgs(node));
            }

            Invalidate();
            return true;
        }

        public void ClearSearchSelection()
        {
            if (_searchSelectedNode == null)
            {
                return;
            }

            _searchSelectedNode = null;
            Invalidate();
        }

        private void ClearSearchSelectionFromInteraction()
        {
            ClearSearchSelection();
        }

        private bool NodeMatchesSearch(GraphNode node, string loweredQuery)
        {
            if (node.DisplayName.ToLowerInvariant().Contains(loweredQuery))
            {
                return true;
            }

            if (!node.Revealed)
            {
                return false;
            }

            string placed = GetNodePlacement(node);
            return placed != "" && placed.ToLowerInvariant().Contains(loweredQuery);
        }

        public void GetProgressCounts(out int revealed, out int total)
        {
            total = _nodes.Count;
            revealed = _nodes.Count(node => node.Revealed);
        }

        public void GetTrackedInventory(
            out HashSet<string> acquired,
            out Dictionary<string, int> grantCounts)
        {
            GetAcquiredState(out acquired, out grantCounts);
        }

        public string GetHoverDetailText(GraphNode node)
        {
            if (node == null)
            {
                return "";
            }

            if (_shiftHoverMode)
            {
                return GetDescendantChainHoverText(node);
            }

            return GetLinkedRequirementHoverText(node);
        }

        private void NotifyInventoryChanged(string placement)
        {
            if (InventoryChanged != null)
            {
                InventoryChanged(this, new GraphInventoryEventArgs(placement));
            }
        }

        private void NotifyInventoryFullRefresh()
        {
            if (InventoryChanged != null)
            {
                InventoryChanged(this, new GraphInventoryEventArgs(true));
            }
        }

        private void NotifyProgressChanged()
        {
            if (ProgressChanged != null)
            {
                ProgressChanged(this, EventArgs.Empty);
            }
        }

        public GraphCanvas()
        {
            DoubleBuffered = true;
            BackColor = IsDesignEnvironment()
                ? Color.FromArgb(32, 32, 32)
                : UiTheme.Current.PanelBackColor;
            Cursor = Cursors.Hand;

            if (IsDesignEnvironment())
            {
                return;
            }

            MouseDown += GraphCanvas_MouseDown;
            MouseMove += GraphCanvas_MouseMove;
            MouseUp += GraphCanvas_MouseUp;
            MouseWheel += GraphCanvas_MouseWheel;
            MouseLeave += GraphCanvas_MouseLeave;
        }

        private void SetHoveredNode(GraphNode hit)
        {
            bool shiftMode = (Control.ModifierKeys & Keys.Shift) == Keys.Shift;
            if (_hoveredNode == hit && _shiftHoverMode == shiftMode)
            {
                return;
            }

            _hoveredNode = hit;
            _shiftHoverMode = shiftMode;
            _highlightedDescendants = shiftMode && hit != null
                ? GetOutgoingDescendantNodes(hit)
                : new HashSet<GraphNode>();
            Invalidate();

            if (NodeHovered != null)
            {
                NodeHovered(this, new GraphNodeEventArgs(hit));
            }
        }

        private void GraphCanvas_MouseLeave(object sender, EventArgs e)
        {
            if (_minimapPanning)
            {
                EndMinimapPan();
            }

            SetHoveredNode(null);
        }

        public string GetLinkedRequirementHoverText(GraphNode target)
        {
            if (target == null || _logicData == null)
            {
                return "";
            }

            List<List<string>> sets;
            if (!_logicData.LocationPrereqs.TryGetValue(target.DisplayName, out sets) || sets.Count == 0)
            {
                if (_rootColumnLocations.Contains(target.DisplayName))
                {
                    return "Available at start (column 0).";
                }

                return "No logic requirements.";
            }

            HashSet<string> acquired;
            Dictionary<string, int> grantCounts;
            GetAcquiredState(out acquired, out grantCounts);
            HashSet<GraphNode> incomingParents = GetIncomingParentNodes(target);

            StringBuilder satisfiedText = new StringBuilder();
            StringBuilder unsatisfiedText = new StringBuilder();
            bool firstSatisfied = true;
            bool firstUnsatisfied = true;

            foreach (List<string> set in sets)
            {
                if (_logicData.IsPrereqSetSatisfied(set, acquired, grantCounts))
                {
                    List<string> linkedChecks = incomingParents
                        .Where(parent =>
                        {
                            string item = GetGrantedItem(parent);
                            return item != ""
                                && _logicData.ParentItemUnlocksLocationForSet(
                                    target.DisplayName,
                                    item,
                                    set,
                                    acquired,
                                    grantCounts);
                        })
                        .Select(parent => parent.DisplayName)
                        .Distinct()
                        .OrderBy(name => name)
                        .ToList();

                    if (linkedChecks.Count == 0 && incomingParents.Count > 0)
                    {
                        continue;
                    }

                    if (!firstSatisfied)
                    {
                        satisfiedText.AppendLine("── OR ──");
                    }

                    firstSatisfied = false;
                    satisfiedText.AppendLine(LogicUsefulnessResult.FormatPrereqSetDisplay(set));
                    if (linkedChecks.Count > 0)
                    {
                        satisfiedText.Append("  \u2190 ");
                        satisfiedText.AppendLine(string.Join(", ", linkedChecks));
                    }
                }
                else
                {
                    List<string> missing = _logicData.GetMissingPrereqsInSet(set, acquired, grantCounts);
                    if (missing.Count == 0)
                    {
                        continue;
                    }

                    if (!firstUnsatisfied)
                    {
                        unsatisfiedText.AppendLine("── OR ──");
                    }

                    firstUnsatisfied = false;
                    unsatisfiedText.AppendLine(LogicUsefulnessResult.FormatPrereqSetDisplay(set));
                    unsatisfiedText.Append("  missing: ");
                    unsatisfiedText.AppendLine(string.Join(", ", missing));
                }
            }

            if (satisfiedText.Length == 0 && unsatisfiedText.Length == 0)
            {
                if (_rootColumnLocations.Contains(target.DisplayName))
                {
                    return "Available at start (column 0).";
                }

                if (incomingParents.Count == 0)
                {
                    return "No linked checks from the left.";
                }

                return "No linked requirement groups from the left.";
            }

            StringBuilder text = new StringBuilder();
            if (satisfiedText.Length > 0)
            {
                text.AppendLine("Satisfied:");
                text.Append(satisfiedText.ToString().TrimEnd());
            }

            if (unsatisfiedText.Length > 0)
            {
                if (text.Length > 0)
                {
                    text.AppendLine();
                    text.AppendLine();
                }

                text.AppendLine("Not yet:");
                text.Append(unsatisfiedText.ToString().TrimEnd());
            }

            return text.ToString().TrimEnd();
        }

        private string GetDescendantChainHoverText(GraphNode source)
        {
            HashSet<GraphNode> descendants = GetOutgoingDescendantNodes(source);
            if (descendants.Count == 0)
            {
                return "Unlocks: (none on graph yet)";
            }

            List<string> names = descendants
                .OrderBy(node => node.Column)
                .ThenBy(node => node.Y)
                .Select(node => node.DisplayName)
                .ToList();

            return "Unlocks:\r\n  " + source.DisplayName + "\r\n  \u2192 "
                + string.Join("\r\n  \u2192 ", names);
        }

        private HashSet<GraphNode> GetOutgoingDescendantNodes(GraphNode source)
        {
            HashSet<GraphNode> descendants = new HashSet<GraphNode>();
            if (source == null)
            {
                return descendants;
            }

            Queue<GraphNode> queue = new Queue<GraphNode>();
            queue.Enqueue(source);

            while (queue.Count > 0)
            {
                GraphNode current = queue.Dequeue();
                foreach (GraphEdge edge in _edges)
                {
                    if (edge.From != current || edge.To == null || descendants.Contains(edge.To))
                    {
                        continue;
                    }

                    descendants.Add(edge.To);
                    queue.Enqueue(edge.To);
                }
            }

            return descendants;
        }

        internal void ResetGraph(
            LogicUsefulnessResult logicData,
            Dictionary<string, string> placements,
            string[] itemNames)
        {
            _logicData = logicData;
            _placements = placements ?? new Dictionary<string, string>();
            _nodes.Clear();
            _edges.Clear();
            _nodeLookup.Clear();
            _rootColumnLocations.Clear();
            _edgeKeys.Clear();
            _nodesWithIncomingEdge.Clear();
            _locationsNotOnGraph.Clear();
            ResetAcquiredCache();
            ResetRoundedNodePath();
            _pendingNode = null;
            _hoveredNode = null;
            _searchSelectedNode = null;
            _panning = false;
            _viewOffset = PointF.Empty;

            using (Graphics measureGraphics = CreateGraphics())
            {
                ComputeUniformNodeSize(measureGraphics, itemNames);
            }

            AddStartNode("Deku Mask");
            AddStartNode("Song of Healing");

            if (_logicData != null)
            {
                foreach (string location in _logicData.GetStartingColumnLocations())
                {
                    if (_startingInventory.Contains(location))
                    {
                        continue;
                    }

                    AddRootLocationNode(location);
                }

                RebuildLocationsNotOnGraph();
            }

            LayoutGraph();
            Invalidate();
            NotifyInventoryFullRefresh();
            NotifyProgressChanged();
        }

        private void AddStartNode(string itemName)
        {
            string key = "start:" + itemName;
            GraphNode node = new GraphNode
            {
                Key = key,
                DisplayName = itemName,
                Kind = GraphNodeKind.StartItem,
                Column = 0,
                IsRootColumn = true
            };
            _nodes.Add(node);
            _nodeLookup[key] = node;
            _rootColumnLocations.Add(itemName);
        }

        private void AddRootLocationNode(string location)
        {
            string key = "loc:" + location;
            GraphNode node = new GraphNode
            {
                Key = key,
                DisplayName = location,
                Kind = GraphNodeKind.Location,
                Column = 0,
                IsRootColumn = true
            };
            _nodes.Add(node);
            _nodeLookup[key] = node;
            _rootColumnLocations.Add(location);
        }

        private PointF ToGraphPoint(Point clientPoint)
        {
            return new PointF(clientPoint.X - _viewOffset.X, clientPoint.Y - _viewOffset.Y);
        }

        private GraphNode HitTest(PointF graphPoint)
        {
            return _nodes
                .OrderByDescending(node => node.Column)
                .FirstOrDefault(node => node.Bounds.Contains(graphPoint));
        }

        private void GraphCanvas_MouseDown(object sender, MouseEventArgs e)
        {
            _mouseDownPoint = e.Location;

            if (e.Button == MouseButtons.Left && TryBeginMinimapPan(e.Location))
            {
                return;
            }

            if (e.Button == MouseButtons.Right)
            {
                GraphNode hit = HitTest(ToGraphPoint(e.Location));
                if (hit != null)
                {
                    ClearSearchSelectionFromInteraction();
                    ToggleNodeMark(hit);
                }

                return;
            }

            if (e.Button == MouseButtons.Middle)
            {
                BeginPan(e.Location);
                return;
            }

            if (e.Button != MouseButtons.Left)
            {
                return;
            }

            GraphNode hitNode = HitTest(ToGraphPoint(e.Location));
            if (hitNode != null)
            {
                _pendingNode = hitNode;
                _panning = false;
            }
            else
            {
                BeginPan(e.Location);
            }
        }

        private void BeginPan(Point location)
        {
            _pendingNode = null;
            _panning = true;
            _mouseDownPoint = location;
            _panOffsetStart = _viewOffset;
            Cursor = Cursors.SizeAll;
            Capture = true;
        }

        private void GraphCanvas_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.None)
            {
                GraphNode hit = HitTest(ToGraphPoint(e.Location));
                SetHoveredNode(hit);
            }

            if (_minimapPanning && e.Button == MouseButtons.Left)
            {
                PanToMinimapPoint(e.Location);
                return;
            }

            if (e.Button == MouseButtons.Left || e.Button == MouseButtons.Middle)
            {
                if (!_panning && _pendingNode != null)
                {
                    int dx = e.X - _mouseDownPoint.X;
                    int dy = e.Y - _mouseDownPoint.Y;
                    if (Math.Abs(dx) > DragThreshold || Math.Abs(dy) > DragThreshold)
                    {
                        BeginPan(_mouseDownPoint);
                    }
                }

                if (_panning)
                {
                    _viewOffset = new PointF(
                        _panOffsetStart.X + (e.X - _mouseDownPoint.X),
                        _panOffsetStart.Y + (e.Y - _mouseDownPoint.Y));
                    Invalidate();
                }
            }
        }

        private void GraphCanvas_MouseUp(object sender, MouseEventArgs e)
        {
            if (_minimapPanning && e.Button == MouseButtons.Left)
            {
                EndMinimapPan();
                return;
            }

            if (e.Button == MouseButtons.Middle || (_panning && e.Button == MouseButtons.Left))
            {
                EndPan();
                return;
            }

            if (e.Button != MouseButtons.Left || _panning)
            {
                EndPan();
                return;
            }

            if (_pendingNode == null)
            {
                return;
            }

            GraphNode hit = _pendingNode;
            _pendingNode = null;

            ClearSearchSelectionFromInteraction();
            MarkNode(hit);
        }

        private void ToggleNodeMark(GraphNode node)
        {
            if (node.Revealed)
            {
                UnmarkNode(node);
            }
            else
            {
                MarkNode(node);
            }
        }

        private void MarkNode(GraphNode hit)
        {
            hit.Revealed = true;
            hit.Expanded = true;

            ApplyMarkToAcquiredCache(hit);
            RefreshAfterMark(hit);

            if (NodeSelected != null)
            {
                NodeSelected(this, new GraphNodeEventArgs(hit));
            }

            Invalidate();
            NotifyInventoryChanged(GetNodePlacement(hit));
            NotifyProgressChanged();
        }

        private void UnmarkNode(GraphNode node)
        {
            string placement = GetNodePlacement(node);
            node.Revealed = false;
            node.Expanded = false;

            ResetAcquiredCache();
            RemoveEdgesFrom(node);

            RemoveOrphanNodes();
            RefreshAfterUnmark();

            if (NodeSelected != null)
            {
                NodeSelected(this, new GraphNodeEventArgs(node));
            }

            Invalidate();
            NotifyInventoryChanged(placement);
            NotifyProgressChanged();
        }

        private void RemoveOrphanNodes()
        {
            List<GraphNode> orphans = new List<GraphNode>();

            foreach (GraphNode node in _nodes)
            {
                if (node.Column > 0 && !_nodesWithIncomingEdge.Contains(node.Key))
                {
                    orphans.Add(node);
                }
            }

            foreach (GraphNode orphan in orphans)
            {
                _edges.RemoveAll(edge => edge.From == orphan || edge.To == orphan);
                _nodeLookup.Remove(orphan.Key);
                _nodes.Remove(orphan);

                if (orphan.Kind == GraphNodeKind.Location
                    && !_rootColumnLocations.Contains(orphan.DisplayName))
                {
                    _locationsNotOnGraph.Add(orphan.DisplayName);
                }
            }

            if (orphans.Count > 0)
            {
                RebuildEdgeIndex();
            }
        }

        private void EndPan()
        {
            _panning = false;
            if (!_minimapPanning)
            {
                Cursor = Cursors.Hand;
                Capture = false;
            }
        }

        private void EndMinimapPan()
        {
            _minimapPanning = false;
            Cursor = Cursors.Hand;
            Capture = false;
        }

        private void GraphCanvas_MouseWheel(object sender, MouseEventArgs e)
        {
            _viewOffset = new PointF(
                _viewOffset.X,
                _viewOffset.Y + e.Delta / 3f);
            Invalidate();
        }

        private string GetNodePlacement(GraphNode node)
        {
            string placed;
            if (_placements != null && _placements.TryGetValue(node.DisplayName, out placed))
            {
                return placed ?? "";
            }

            return "";
        }

        private void GetAcquiredState(
            out HashSet<string> acquired,
            out Dictionary<string, int> grantCounts)
        {
            acquired = new HashSet<string>(StringComparer.Ordinal);
            grantCounts = new Dictionary<string, int>(StringComparer.Ordinal);

            foreach (GraphNode node in _nodes)
            {
                if (!node.Revealed)
                {
                    continue;
                }

                string placed = GetNodePlacement(node);
                if (placed == "")
                {
                    continue;
                }

                string normalized = ItemGrantCounts.GetBaseItemName(placed);
                if (normalized == "")
                {
                    normalized = placed;
                }

                acquired.Add(normalized);

                int quantity = ItemGrantCounts.GetGrantQuantity(node.DisplayName, placed);
                if (quantity <= 0)
                {
                    quantity = 1;
                }

                int count;
                if (grantCounts.TryGetValue(normalized, out count))
                {
                    grantCounts[normalized] = count + quantity;
                }
                else
                {
                    grantCounts[normalized] = quantity;
                }
            }
        }

        private void ResetAcquiredCache()
        {
            _hasAcquiredCache = false;
            _cachedAcquiredItems = null;
            _cachedGrantCounts = null;
        }

        private void ApplyMarkToAcquiredCache(GraphNode node)
        {
            string placed = GetNodePlacement(node);
            if (placed == "")
            {
                return;
            }

            if (!_hasAcquiredCache)
            {
                return;
            }

            string normalized = ItemGrantCounts.GetBaseItemName(placed);
            if (normalized == "")
            {
                normalized = placed;
            }

            _cachedAcquiredItems.Add(normalized);

            int quantity = ItemGrantCounts.GetGrantQuantity(node.DisplayName, placed);
            if (quantity <= 0)
            {
                quantity = 1;
            }

            int count;
            if (_cachedGrantCounts.TryGetValue(normalized, out count))
            {
                _cachedGrantCounts[normalized] = count + quantity;
            }
            else
            {
                _cachedGrantCounts[normalized] = quantity;
            }
        }

        private AcquiredSnapshot CreateAcquiredSnapshot()
        {
            if (!_hasAcquiredCache)
            {
                GetAcquiredState(out _cachedAcquiredItems, out _cachedGrantCounts);
                _hasAcquiredCache = true;
            }

            return new AcquiredSnapshot
            {
                Acquired = _cachedAcquiredItems,
                GrantCounts = _cachedGrantCounts
            };
        }

        private void RebuildLocationsNotOnGraph()
        {
            _locationsNotOnGraph.Clear();
            if (_logicData == null || _logicData.LocationPrereqs == null)
            {
                return;
            }

            foreach (string location in _logicData.LocationPrereqs.Keys)
            {
                if (!_rootColumnLocations.Contains(location))
                {
                    _locationsNotOnGraph.Add(location);
                }
            }

            foreach (GraphNode node in _nodes)
            {
                _locationsNotOnGraph.Remove(node.DisplayName);
            }
        }

        private static string MakeEdgeKey(GraphNode from, GraphNode to)
        {
            return from.Key + "\0" + to.Key;
        }

        private void RebuildEdgeIndex()
        {
            _edgeKeys.Clear();
            _nodesWithIncomingEdge.Clear();
            _incomingParentsByChildKey.Clear();

            foreach (GraphEdge edge in _edges)
            {
                _edgeKeys.Add(MakeEdgeKey(edge.From, edge.To));
                _nodesWithIncomingEdge.Add(edge.To.Key);

                List<GraphNode> parents;
                if (!_incomingParentsByChildKey.TryGetValue(edge.To.Key, out parents))
                {
                    parents = new List<GraphNode>();
                    _incomingParentsByChildKey[edge.To.Key] = parents;
                }

                parents.Add(edge.From);
            }
        }

        private bool AddEdge(GraphNode from, GraphNode to)
        {
            string key = MakeEdgeKey(from, to);
            if (_edgeKeys.Contains(key))
            {
                return false;
            }

            _edges.Add(new GraphEdge { From = from, To = to });
            _edgeKeys.Add(key);
            _nodesWithIncomingEdge.Add(to.Key);
            return true;
        }

        private void RemoveEdgesFrom(GraphNode node)
        {
            _edges.RemoveAll(edge =>
            {
                if (edge.From != node)
                {
                    return false;
                }

                _edgeKeys.Remove(MakeEdgeKey(edge.From, edge.To));
                return true;
            });

            RebuildEdgeIndex();
        }

        private IEnumerable<string> GetCandidateLocationsForGrantedItem(string item)
        {
            if (_logicData == null || _logicData.ReverseMap == null || item == "")
            {
                yield break;
            }

            HashSet<string> seen = new HashSet<string>(StringComparer.Ordinal);

            foreach (string mapKey in GetReverseMapKeysForItem(item))
            {
                List<string> locations;
                if (!_logicData.ReverseMap.TryGetValue(mapKey, out locations))
                {
                    continue;
                }

                foreach (string location in locations)
                {
                    if (seen.Add(location))
                    {
                        yield return location;
                    }
                }
            }
        }

        private IEnumerable<string> GetReverseMapKeysForItem(string item)
        {
            yield return item;

            string baseName = ItemGrantCounts.GetBaseItemName(item);
            if (baseName != "" && !string.Equals(baseName, item, StringComparison.Ordinal))
            {
                yield return baseName;
            }

            if (item == "Giant Wallet" || item == "Adult Wallet")
            {
                yield return "Rupees";
            }
        }

        private string GetGrantedItem(GraphNode node)
        {
            return GetNodePlacement(node);
        }

        private List<string> GetChildLocations(GraphNode parent, AcquiredSnapshot snapshot)
        {
            if (_logicData == null || _logicData.LocationPrereqs == null)
            {
                return new List<string>();
            }

            string item = GetGrantedItem(parent);
            if (item == "")
            {
                return new List<string>();
            }

            List<string> matches = new List<string>();

            foreach (string location in GetCandidateLocationsForGrantedItem(item))
            {
                if (_rootColumnLocations.Contains(location))
                {
                    continue;
                }

                string childKey = "loc:" + location;
                GraphNode existingChild;
                if (_nodeLookup.TryGetValue(childKey, out existingChild) && EdgeExists(parent, existingChild))
                {
                    continue;
                }

                if (!_logicData.IsLocationReachable(
                    location,
                    snapshot.Acquired,
                    snapshot.GrantCounts))
                {
                    continue;
                }

                if (!_logicData.ParentItemUnlocksLocation(
                    location,
                    item,
                    snapshot.Acquired,
                    snapshot.GrantCounts))
                {
                    continue;
                }

                matches.Add(location);
            }

            return matches;
        }

        private void RefreshAfterMark(GraphNode marked)
        {
            AcquiredSnapshot snapshot = CreateAcquiredSnapshot();
            string grantedItem = GetGrantedItem(marked);
            bool structureChanged = SyncChildren(marked, snapshot);

            if (SyncMissingReachableLocations(snapshot, grantedItem))
            {
                structureChanged = true;
            }

            if (structureChanged)
            {
                RebuildEdgeIndex();
                LayoutGraph();
            }
        }

        private void RefreshAfterUnmark()
        {
            AcquiredSnapshot snapshot = CreateAcquiredSnapshot();
            int edgeCountBefore = _edges.Count;

            PruneBackwardEdges();
            PruneInvalidEdges(snapshot);
            RebuildEdgeIndex();

            if (_edges.Count != edgeCountBefore)
            {
                LayoutGraph();
            }
        }

        private void PruneBackwardEdges()
        {
            _edges.RemoveAll(edge => edge.To.Column <= edge.From.Column);
        }

        private void PruneInvalidEdges(AcquiredSnapshot snapshot)
        {
            _edges.RemoveAll(edge =>
            {
                string item = GetGrantedItem(edge.From);
                if (item == "")
                {
                    return true;
                }

                return !_logicData.ParentItemUnlocksLocation(
                    edge.To.DisplayName,
                    item,
                    snapshot.Acquired,
                    snapshot.GrantCounts);
            });
        }

        private int ComputeColumnForLocation(
            string location,
            HashSet<string> acquired,
            Dictionary<string, int> grantCounts)
        {
            if (_rootColumnLocations.Contains(location)
                || (_logicData != null && _logicData.IsWalletOnlyReachableAtStart(location)))
            {
                return 0;
            }

            if (_logicData == null || !_logicData.IsLocationReachable(location, acquired, grantCounts))
            {
                return -1;
            }

            int maxPrereqColumn = GetMaxPrerequisiteColumnForLocation(location, acquired, grantCounts);
            if (maxPrereqColumn < 0)
            {
                return 1;
            }

            return maxPrereqColumn + 1;
        }

        private int GetMaxPrerequisiteColumnForLocation(
            string location,
            HashSet<string> acquired,
            Dictionary<string, int> grantCounts)
        {
            List<List<string>> sets;
            if (!_logicData.LocationPrereqs.TryGetValue(location, out sets) || sets.Count == 0)
            {
                return -1;
            }

            int deepest = -1;

            foreach (List<string> set in sets)
            {
                if (!_logicData.IsPrereqSetSatisfied(set, acquired, grantCounts))
                {
                    continue;
                }

                if (_logicData.IsRupeesOnlySet(set))
                {
                    int rupeesRequired = LogicUsefulnessResult.GetMaxRupeesInSet(set);
                    int walletColumn = GetMaxColumnForWalletEnablingRupees(rupeesRequired);
                    deepest = Math.Max(deepest, walletColumn);
                    continue;
                }

                int setMax = 0;
                foreach (string rawPrereq in set)
                {
                    int countNeeded;
                    string itemName;
                    if (!LogicUsefulnessResult.TryParsePrereq(rawPrereq, out countNeeded, out itemName))
                    {
                        continue;
                    }

                    if (itemName == "Rupees")
                    {
                        continue;
                    }

                    int itemColumn = GetMaxColumnForGrantedItem(itemName);
                    setMax = Math.Max(setMax, Math.Max(itemColumn, 0));
                }

                deepest = Math.Max(deepest, setMax);
            }

            return deepest;
        }

        private int GetMaxColumnForWalletEnablingRupees(int rupeesRequired)
        {
            int maxColumn = 0;

            foreach (string walletItem in new[] { "Giant Wallet", "Adult Wallet" })
            {
                if (!LogicUsefulnessResult.WalletItemEnablesRupees(walletItem, rupeesRequired))
                {
                    continue;
                }

                int walletColumn = GetMaxColumnForGrantedItem(walletItem);
                maxColumn = Math.Max(maxColumn, Math.Max(walletColumn, 0));
            }

            return maxColumn;
        }

        private int GetMaxColumnForGrantedItem(string item)
        {
            int maxColumn = -1;

            foreach (GraphNode node in _nodes)
            {
                if (!node.Revealed)
                {
                    continue;
                }

                if (!ItemGrantCounts.ItemsMatch(item, GetGrantedItem(node)))
                {
                    continue;
                }

                maxColumn = Math.Max(maxColumn, node.Column);
            }

            return maxColumn;
        }

        private bool SyncMissingReachableLocations(AcquiredSnapshot snapshot, string triggerItem = null)
        {
            if (_logicData == null || _locationsNotOnGraph.Count == 0)
            {
                return false;
            }

            bool changed = false;
            IEnumerable<string> candidates;

            if (!string.IsNullOrEmpty(triggerItem))
            {
                candidates = GetCandidateLocationsForGrantedItem(triggerItem);
            }
            else
            {
                candidates = _locationsNotOnGraph;
            }

            foreach (string location in candidates)
            {
                if (!_locationsNotOnGraph.Contains(location))
                {
                    continue;
                }

                if (!_logicData.IsLocationReachable(
                    location,
                    snapshot.Acquired,
                    snapshot.GrantCounts))
                {
                    continue;
                }

                GraphNode parent = FindParentForLocation(location, snapshot);
                if (parent == null)
                {
                    continue;
                }

                if (EnsureChildLinked(parent, location, snapshot))
                {
                    changed = true;
                }
            }

            return changed;
        }

        private GraphNode FindParentForLocation(
            string location,
            AcquiredSnapshot snapshot)
        {
            GraphNode best = null;

            foreach (GraphNode parent in _nodes)
            {
                if (!parent.Expanded)
                {
                    continue;
                }

                string item = GetGrantedItem(parent);
                if (item == ""
                    || !_logicData.ParentItemUnlocksLocation(
                        location,
                        item,
                        snapshot.Acquired,
                        snapshot.GrantCounts))
                {
                    continue;
                }

                if (best == null || parent.Column < best.Column)
                {
                    best = parent;
                }
            }

            if (best != null)
            {
                return best;
            }

            string prereqItem;
            if (!_logicData.TryGetSatisfiedPrereqItem(
                location,
                snapshot.Acquired,
                out prereqItem,
                snapshot.GrantCounts))
            {
                return FindFallbackParentForLocation(location, snapshot);
            }

            foreach (GraphNode parent in _nodes)
            {
                if (!parent.Expanded)
                {
                    continue;
                }

                string parentItem = GetGrantedItem(parent);
                if (!ItemGrantCounts.ItemsMatch(prereqItem, parentItem)
                    || !_logicData.ParentItemUnlocksLocation(
                        location,
                        parentItem,
                        snapshot.Acquired,
                        snapshot.GrantCounts))
                {
                    continue;
                }

                return parent;
            }

            return FindFallbackParentForLocation(location, snapshot);
        }

        private GraphNode FindFallbackParentForLocation(
            string location,
            AcquiredSnapshot snapshot)
        {
            int targetColumn = GetMaxPrerequisiteColumnForLocation(
                location,
                snapshot.Acquired,
                snapshot.GrantCounts);
            if (targetColumn < 0)
            {
                targetColumn = 0;
            }

            GraphNode parent = null;
            GraphNode columnZeroParent = null;

            foreach (GraphNode node in _nodes)
            {
                if (!node.Expanded)
                {
                    continue;
                }

                if (node.Column == targetColumn && (parent == null || node.Y < parent.Y))
                {
                    parent = node;
                }

                if (node.Column == 0 && (columnZeroParent == null || node.Y < columnZeroParent.Y))
                {
                    columnZeroParent = node;
                }
            }

            if (parent != null)
            {
                return parent;
            }

            return columnZeroParent;
        }

        private bool SyncChildren(GraphNode parent, AcquiredSnapshot snapshot)
        {
            List<string> targets = GetChildLocations(parent, snapshot);
            bool changed = false;

            foreach (string location in targets)
            {
                if (EnsureChildLinked(parent, location, snapshot))
                {
                    changed = true;
                }
            }

            return changed;
        }

        private bool EnsureChildLinked(
            GraphNode parent,
            string location,
            AcquiredSnapshot snapshot)
        {
            string key = "loc:" + location;
            GraphNode child;
            int childColumn = ComputeColumnForLocation(
                location,
                snapshot.Acquired,
                snapshot.GrantCounts);
            if (childColumn < 0)
            {
                childColumn = parent.Column + 1;
            }

            bool changed = false;
            bool isNewChild = false;

            if (!_nodeLookup.TryGetValue(key, out child))
            {
                isNewChild = true;
                child = new GraphNode
                {
                    Key = key,
                    DisplayName = location,
                    Kind = GraphNodeKind.Location,
                    Column = childColumn
                };
                _nodes.Add(child);
                _nodeLookup[key] = child;
                _locationsNotOnGraph.Remove(location);
                changed = true;
            }

            bool shouldLink = isNewChild || child.Column > parent.Column;
            if (shouldLink && AddEdge(parent, child))
            {
                changed = true;
            }

            return changed;
        }

        private bool EdgeExists(GraphNode from, GraphNode to)
        {
            return _edgeKeys.Contains(MakeEdgeKey(from, to));
        }

        private string GetNodeLabel(GraphNode node)
        {
            if (!node.Revealed)
            {
                return node.DisplayName;
            }

            string placed;
            if (_placements != null && _placements.TryGetValue(node.DisplayName, out placed)
                && !string.IsNullOrEmpty(placed))
            {
                return node.DisplayName + "\n" + placed;
            }

            return node.DisplayName;
        }

        private void ComputeUniformNodeSize(Graphics graphics, string[] itemNames)
        {
            Font font = UiTheme.Current.BaseFont;
            float maxTextWidth = 0f;
            float maxTextHeight = 0f;

            if (itemNames != null && itemNames.Length > 0)
            {
                foreach (string slot in itemNames)
                {
                    SizeF singleLine = graphics.MeasureString(slot, font);
                    maxTextWidth = Math.Max(maxTextWidth, singleLine.Width);
                    maxTextHeight = Math.Max(maxTextHeight, singleLine.Height);

                    foreach (string placed in itemNames)
                    {
                        SizeF twoLine = graphics.MeasureString(slot + "\n" + placed, font);
                        maxTextWidth = Math.Max(maxTextWidth, twoLine.Width);
                        maxTextHeight = Math.Max(maxTextHeight, twoLine.Height);
                    }
                }
            }

            _uniformNodeWidth = Math.Max(MinNodeWidth, maxTextWidth + NodePaddingX * 2f);
            _uniformNodeHeight = Math.Max(MinNodeHeight, maxTextHeight + NodePaddingY * 2f);
        }

        private void MeasureNode(GraphNode node)
        {
            node.Width = _uniformNodeWidth;
            node.Height = _uniformNodeHeight;
        }

        private void LayoutGraph()
        {
            foreach (GraphNode node in _nodes)
            {
                MeasureNode(node);
            }

            Dictionary<int, List<GraphNode>> columns = _nodes
                .Select((node, index) => new { node, index })
                .GroupBy(pair => pair.node.Column)
                .ToDictionary(
                    group => group.Key,
                    group => group
                        .OrderBy(pair => pair.index)
                        .Select(pair => pair.node)
                        .ToList());

            float[] columnWidths = new float[columns.Count == 0 ? 1 : columns.Keys.Max() + 1];
            foreach (KeyValuePair<int, List<GraphNode>> column in columns)
            {
                columnWidths[column.Key] = column.Value.Max(node => node.Width);
            }

            float[] columnX = new float[columnWidths.Length];
            float x = GraphMargin;
            for (int c = 0; c < columnWidths.Length; c++)
            {
                columnX[c] = x;
                x += columnWidths[c] + ColumnGap;
            }

            foreach (GraphNode node in _nodes)
            {
                node.X = columnX[node.Column];
                node.Bounds = new RectangleF(node.X, node.Y, node.Width, node.Height);
            }

            ReflowColumnPositions(columns);
        }

        private void ReflowColumnPositions(Dictionary<int, List<GraphNode>> columns)
        {
            foreach (KeyValuePair<int, List<GraphNode>> column in columns.OrderBy(pair => pair.Key))
            {
                float y = GraphMargin;
                foreach (GraphNode node in column.Value)
                {
                    node.Y = y;
                    node.Bounds = new RectangleF(node.X, node.Y, node.Width, node.Height);
                    y += node.Height + RowGap;
                }
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            RectangleF visibleGraphBounds = GetVisibleGraphBounds();
            bool cullOffscreen = _nodes.Count >= 40;

            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
            e.Graphics.TranslateTransform(_viewOffset.X, _viewOffset.Y);

            Color accent = UiTheme.Current.TabSelectedAccentColor;
            Color edgeColor = UiTheme.Current.HintForeColor;
            HashSet<GraphNode> incomingParents = _shiftHoverMode
                ? new HashSet<GraphNode>()
                : GetIncomingParentNodes(_hoveredNode);

            foreach (GraphEdge edge in _edges)
            {
                if (cullOffscreen && !EdgeIntersectsVisible(edge, visibleGraphBounds))
                {
                    continue;
                }

                bool highlightIncoming = !_shiftHoverMode
                    && _hoveredNode != null
                    && edge.To == _hoveredNode;
                bool highlightOutgoing = _shiftHoverMode
                    && _hoveredNode != null
                    && (_highlightedDescendants.Contains(edge.To)
                        || edge.From == _hoveredNode);
                bool highlight = highlightIncoming || highlightOutgoing;
                Color color = highlight ? accent : edgeColor;
                float width = highlight ? 2.5f : 1.5f;

                PointF start = new PointF(
                    edge.From.Bounds.Right,
                    edge.From.Bounds.Top + edge.From.Bounds.Height / 2f);
                PointF end = new PointF(
                    edge.To.Bounds.Left,
                    edge.To.Bounds.Top + edge.To.Bounds.Height / 2f);

                using (Pen edgePen = new Pen(color, width))
                {
                    e.Graphics.DrawLine(edgePen, start, end);
                }
            }

            foreach (GraphNode node in _nodes)
            {
                if (cullOffscreen && !visibleGraphBounds.IntersectsWith(node.Bounds))
                {
                    continue;
                }

                DrawNode(
                    e.Graphics,
                    node,
                    incomingParents,
                    _shiftHoverMode ? _highlightedDescendants : null);
            }

            e.Graphics.ResetTransform();
            DrawMinimap(e.Graphics);
        }

        private RectangleF GetVisibleGraphBounds()
        {
            const float margin = 48f;
            return new RectangleF(
                -_viewOffset.X - margin,
                -_viewOffset.Y - margin,
                ClientSize.Width + margin * 2f,
                ClientSize.Height + margin * 2f);
        }

        private static bool EdgeIntersectsVisible(GraphEdge edge, RectangleF visible)
        {
            RectangleF fromBounds = edge.From.Bounds;
            RectangleF toBounds = edge.To.Bounds;
            float minX = Math.Min(fromBounds.Left, toBounds.Left);
            float minY = Math.Min(fromBounds.Top, toBounds.Top);
            float maxX = Math.Max(fromBounds.Right, toBounds.Right);
            float maxY = Math.Max(fromBounds.Bottom, toBounds.Bottom);
            return visible.IntersectsWith(RectangleF.FromLTRB(minX, minY, maxX, maxY));
        }

        private RectangleF GetMinimapBounds()
        {
            return new RectangleF(
                ClientSize.Width - MinimapWidth - MinimapMargin,
                ClientSize.Height - MinimapHeight - MinimapMargin,
                MinimapWidth,
                MinimapHeight);
        }

        private RectangleF GetGraphBounds()
        {
            if (_nodes.Count == 0)
            {
                return new RectangleF(0, 0, 1, 1);
            }

            float minX = _nodes.Min(node => node.Bounds.Left);
            float minY = _nodes.Min(node => node.Bounds.Top);
            float maxX = _nodes.Max(node => node.Bounds.Right);
            float maxY = _nodes.Max(node => node.Bounds.Bottom);
            return RectangleF.FromLTRB(minX, minY, maxX, maxY);
        }

        private bool TryGetMinimapTransform(
            out float scale,
            out float offsetX,
            out float offsetY)
        {
            scale = 1f;
            offsetX = 0f;
            offsetY = 0f;

            if (_nodes.Count == 0)
            {
                return false;
            }

            RectangleF minimapBounds = GetMinimapBounds();
            RectangleF graphBounds = GetGraphBounds();
            if (graphBounds.Width <= 0 || graphBounds.Height <= 0)
            {
                return false;
            }

            float scaleX = (minimapBounds.Width - 8f) / graphBounds.Width;
            float scaleY = (minimapBounds.Height - 8f) / graphBounds.Height;
            scale = Math.Min(scaleX, scaleY);
            offsetX = minimapBounds.X + 4f - graphBounds.X * scale;
            offsetY = minimapBounds.Y + 4f - graphBounds.Y * scale;
            return true;
        }

        private bool IsPointInMinimap(Point clientPoint)
        {
            return _nodes.Count > 0 && GetMinimapBounds().Contains(clientPoint);
        }

        private bool TryBeginMinimapPan(Point clientPoint)
        {
            if (!IsPointInMinimap(clientPoint))
            {
                return false;
            }

            _minimapPanning = true;
            _pendingNode = null;
            _panning = false;
            Cursor = Cursors.SizeAll;
            Capture = true;
            PanToMinimapPoint(clientPoint);
            return true;
        }

        private void PanToMinimapPoint(Point clientPoint)
        {
            float scale;
            float offsetX;
            float offsetY;
            if (!TryGetMinimapTransform(out scale, out offsetX, out offsetY))
            {
                return;
            }

            float graphX = (clientPoint.X - offsetX) / scale;
            float graphY = (clientPoint.Y - offsetY) / scale;

            _viewOffset = new PointF(
                ClientSize.Width / 2f - graphX,
                ClientSize.Height / 2f - graphY);
            Invalidate();
        }

        private void DrawMinimap(Graphics graphics)
        {
            if (_nodes.Count == 0)
            {
                return;
            }

            float scale;
            float offsetX;
            float offsetY;
            if (!TryGetMinimapTransform(out scale, out offsetX, out offsetY))
            {
                return;
            }

            RectangleF minimapBounds = GetMinimapBounds();
            Color accent = UiTheme.Current.TabSelectedAccentColor;
            using (SolidBrush backdrop = new SolidBrush(Color.FromArgb(200, 20, 20, 20)))
            using (Pen borderPen = new Pen(UiTheme.Current.HintForeColor))
            {
                graphics.FillRectangle(backdrop, minimapBounds);
                graphics.DrawRectangle(borderPen, minimapBounds.X, minimapBounds.Y, minimapBounds.Width, minimapBounds.Height);
            }

            foreach (GraphNode node in _nodes)
            {
                RectangleF miniRect = new RectangleF(
                    offsetX + node.Bounds.X * scale,
                    offsetY + node.Bounds.Y * scale,
                    Math.Max(2f, node.Bounds.Width * scale),
                    Math.Max(2f, node.Bounds.Height * scale));

                Color fill = node.Revealed ? accent : Color.FromArgb(120, accent);
                using (SolidBrush brush = new SolidBrush(fill))
                {
                    graphics.FillRectangle(brush, miniRect);
                }
            }

            float viewLeft = -_viewOffset.X;
            float viewTop = -_viewOffset.Y;
            RectangleF viewport = new RectangleF(
                offsetX + viewLeft * scale,
                offsetY + viewTop * scale,
                ClientSize.Width * scale,
                ClientSize.Height * scale);

            using (Pen viewportPen = new Pen(Color.White, 1.5f))
            {
                graphics.DrawRectangle(viewportPen, viewport.X, viewport.Y, viewport.Width, viewport.Height);
            }
        }

        private HashSet<GraphNode> GetIncomingParentNodes(GraphNode hoveredNode)
        {
            HashSet<GraphNode> parents = new HashSet<GraphNode>();
            if (hoveredNode == null)
            {
                return parents;
            }

            List<GraphNode> indexedParents;
            if (_incomingParentsByChildKey.TryGetValue(hoveredNode.Key, out indexedParents))
            {
                foreach (GraphNode parent in indexedParents)
                {
                    parents.Add(parent);
                }

                return parents;
            }

            foreach (GraphEdge edge in _edges)
            {
                if (edge.To == hoveredNode)
                {
                    parents.Add(edge.From);
                }
            }

            return parents;
        }

        private void DrawNode(
            Graphics graphics,
            GraphNode node,
            HashSet<GraphNode> incomingParents,
            HashSet<GraphNode> highlightedDescendants)
        {
            Color accent = UiTheme.Current.TabSelectedAccentColor;
            Color hoverFill = Color.FromArgb(56, accent.R, accent.G, accent.B);
            Color fill;
            Color border;
            Color text;
            bool searchSelected = node == _searchSelectedNode;
            bool hovered = node == _hoveredNode && !searchSelected;
            bool incomingParent = !searchSelected
                && !_shiftHoverMode
                && incomingParents != null
                && incomingParents.Contains(node);
            bool descendantHighlight = !searchSelected
                && highlightedDescendants != null
                && highlightedDescendants.Contains(node);

            if (node.Revealed)
            {
                fill = accent;
                text = Color.White;
            }
            else if (hovered)
            {
                fill = hoverFill;
                text = UiTheme.Current.ForeColor;
            }
            else
            {
                fill = Color.Black;
                text = UiTheme.Current.ForeColor;
            }

            if (searchSelected)
            {
                border = SearchSelectedBorderColor;
            }
            else if (descendantHighlight)
            {
                border = HoverDescendantBorderColor;
            }
            else if (incomingParent)
            {
                border = HoverIncomingParentBorderColor;
            }
            else
            {
                border = accent;
            }

            float borderWidth = searchSelected ? 3f : ((incomingParent || descendantHighlight) ? 2.5f : 2f);

            GraphicsPath nodePath = GetRoundedNodePath();
            Matrix previousTransform = graphics.Transform;
            graphics.TranslateTransform(node.Bounds.X, node.Bounds.Y);

            using (SolidBrush fillBrush = new SolidBrush(fill))
            using (Pen borderPen = new Pen(border, borderWidth))
            {
                graphics.FillPath(fillBrush, nodePath);
                graphics.DrawPath(borderPen, nodePath);
            }

            graphics.Transform = previousTransform;

            string label = GetNodeLabel(node);

            using (StringFormat format = new StringFormat
            {
                Alignment = StringAlignment.Center,
                LineAlignment = StringAlignment.Center,
                Trimming = StringTrimming.None,
                FormatFlags = StringFormatFlags.LineLimit
            })
            using (SolidBrush textBrush = new SolidBrush(text))
            {
                graphics.DrawString(
                    label,
                    UiTheme.Current.BaseFont,
                    textBrush,
                    node.Bounds,
                    format);
            }
        }

        private GraphicsPath GetRoundedNodePath()
        {
            if (_roundedNodePath == null
                || _roundedNodePathWidth != _uniformNodeWidth
                || _roundedNodePathHeight != _uniformNodeHeight)
            {
                ResetRoundedNodePath();
                _roundedNodePath = CreateRoundedRect(
                    new RectangleF(0f, 0f, _uniformNodeWidth, _uniformNodeHeight),
                    8f);
                _roundedNodePathWidth = _uniformNodeWidth;
                _roundedNodePathHeight = _uniformNodeHeight;
            }

            return _roundedNodePath;
        }

        private void ResetRoundedNodePath()
        {
            if (_roundedNodePath != null)
            {
                _roundedNodePath.Dispose();
                _roundedNodePath = null;
            }

            _roundedNodePathWidth = 0f;
            _roundedNodePathHeight = 0f;
        }

        private static GraphicsPath CreateRoundedRect(RectangleF bounds, float radius)
        {
            GraphicsPath path = new GraphicsPath();
            float diameter = radius * 2f;
            path.AddArc(bounds.X, bounds.Y, diameter, diameter, 180, 90);
            path.AddArc(bounds.Right - diameter, bounds.Y, diameter, diameter, 270, 90);
            path.AddArc(bounds.Right - diameter, bounds.Bottom - diameter, diameter, diameter, 0, 90);
            path.AddArc(bounds.X, bounds.Bottom - diameter, diameter, diameter, 90, 90);
            path.CloseFigure();
            return path;
        }

        private static bool IsDesignEnvironment()
        {
            return LicenseManager.UsageMode == LicenseUsageMode.Designtime;
        }
    }
}
