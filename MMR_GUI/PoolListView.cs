using System;
using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class PoolListView
    {
        public static ListView Create()
        {
            ListView list = new ListView
            {
                Dock = DockStyle.Fill,
                View = View.Details,
                FullRowSelect = true,
                HeaderStyle = ColumnHeaderStyle.None,
                BorderStyle = BorderStyle.FixedSingle,
                MultiSelect = false,
                HideSelection = false,
                OwnerDraw = true
            };
            list.Columns.Add("Item", 240);
            AttachThemedDrawing(list, ThemedRowStyle.Standard);
            list.Resize += (sender, args) =>
            {
                if (list.Columns.Count > 0)
                {
                    list.Columns[0].Width = Math.Max(80, list.ClientSize.Width - 4);
                }
            };
            UiTheme.EnableDoubleBuffer(list);
            ApplyTheme(list);
            return list;
        }

        public static void ConfigureDetailsList(ListView list, bool showHeaders)
        {
            list.View = View.Details;
            list.FullRowSelect = true;
            list.MultiSelect = false;
            list.HideSelection = false;
            list.BorderStyle = BorderStyle.FixedSingle;
            list.HeaderStyle = showHeaders
                ? ColumnHeaderStyle.Nonclickable
                : ColumnHeaderStyle.None;
            list.OwnerDraw = true;
            AttachThemedDrawing(list, ThemedRowStyle.Standard);
            UiTheme.EnableDoubleBuffer(list);
            ApplyTheme(list);
        }

        public static void ConfigureIssueList(ListView list)
        {
            list.View = View.Details;
            list.FullRowSelect = true;
            list.MultiSelect = false;
            list.HideSelection = false;
            list.HeaderStyle = ColumnHeaderStyle.Nonclickable;
            list.OwnerDraw = true;
            AttachThemedDrawing(list, ThemedRowStyle.Error);
            UiTheme.EnableDoubleBuffer(list);
            ApplyTheme(list);
        }

        public static void ApplyTheme(ListView list)
        {
            if (list == null)
            {
                return;
            }

            ThemePalette theme = UiTheme.Current;
            list.BackColor = theme.ListEvenRowColor;
            list.ForeColor = theme.ForeColor;
            list.Font = theme.BaseFont;
            UiTheme.ApplyNativeControlTheme(list);
            list.Invalidate();
        }

        public static void SetItems(ListView list, string[] items)
        {
            list.BeginUpdate();
            list.Items.Clear();
            if (items != null)
            {
                foreach (string item in items)
                {
                    list.Items.Add(item);
                }
            }

            if (list.Columns.Count > 0 && list.ClientSize.Width > 0)
            {
                list.Columns[0].Width = list.ClientSize.Width - 4;
            }

            list.EndUpdate();
        }

        private static void AttachThemedDrawing(ListView list, ThemedRowStyle rowStyle)
        {
            list.DrawColumnHeader -= DrawColumnHeader;
            list.DrawItem -= DrawItem;
            list.Paint -= PaintEmptyBackground;

            list.DrawColumnHeader += DrawColumnHeader;
            list.DrawItem += DrawItem;
            list.DrawSubItem += (sender, e) => DrawThemedSubItem(e, rowStyle);
            list.Paint += PaintEmptyBackground;
        }

        private static void PaintEmptyBackground(object sender, PaintEventArgs e)
        {
            ListView list = (ListView)sender;
            ThemePalette theme = UiTheme.Current;
            Rectangle bounds = list.ClientRectangle;
            if (list.Items.Count == 0)
            {
                using (SolidBrush brush = new SolidBrush(theme.ListEvenRowColor))
                {
                    e.Graphics.FillRectangle(brush, bounds);
                }

                return;
            }

            if (list.View != View.Details || list.Items.Count == 0)
            {
                return;
            }

            int bottom = list.Items[list.Items.Count - 1].Bounds.Bottom;
            if (bottom < bounds.Bottom)
            {
                using (SolidBrush brush = new SolidBrush(theme.ListEvenRowColor))
                {
                    e.Graphics.FillRectangle(
                        brush,
                        bounds.Left,
                        bottom,
                        bounds.Width,
                        bounds.Bottom - bottom);
                }
            }
        }

        private static void DrawColumnHeader(object sender, DrawListViewColumnHeaderEventArgs e)
        {
            ThemePalette theme = UiTheme.Current;
            using (SolidBrush backBrush = new SolidBrush(theme.ListHeaderBackColor))
            {
                e.Graphics.FillRectangle(backBrush, e.Bounds);
            }

            using (Pen borderPen = new Pen(theme.BorderColor))
            {
                e.Graphics.DrawLine(
                    borderPen,
                    e.Bounds.Left,
                    e.Bounds.Bottom - 1,
                    e.Bounds.Right,
                    e.Bounds.Bottom - 1);
            }

            TextRenderer.DrawText(
                e.Graphics,
                e.Header.Text,
                e.Font ?? UiTheme.Current.BaseFont,
                e.Bounds,
                theme.ListHeaderForeColor,
                TextFormatFlags.Left | TextFormatFlags.VerticalCenter | TextFormatFlags.EndEllipsis);
        }

        private static void DrawItem(object sender, DrawListViewItemEventArgs e)
        {
            e.DrawDefault = false;
        }

        private static void DrawThemedSubItem(DrawListViewSubItemEventArgs e, ThemedRowStyle rowStyle)
        {
            ThemePalette theme = UiTheme.Current;
            ListView list = e.Item.ListView;
            Color back = e.Item.Selected
                ? theme.ListSelectedBackColor
                : (e.ItemIndex % 2 == 0 ? theme.ListEvenRowColor : theme.ListOddRowColor);
            Color fore = e.Item.Selected
                ? theme.ListSelectedForeColor
                : (rowStyle == ThemedRowStyle.Error ? theme.ErrorForeColor : theme.ForeColor);

            using (SolidBrush brush = new SolidBrush(back))
            {
                e.Graphics.FillRectangle(brush, e.Bounds);
            }

            TextRenderer.DrawText(
                e.Graphics,
                e.SubItem.Text,
                list.Font,
                e.Bounds,
                fore,
                TextFormatFlags.Left | TextFormatFlags.VerticalCenter | TextFormatFlags.EndEllipsis);
        }

        private enum ThemedRowStyle
        {
            Standard,
            Error
        }
    }
}
