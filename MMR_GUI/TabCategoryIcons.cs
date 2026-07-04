using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class TabCategoryIcons
    {
        private static ImageList _imageList;
        private static Font _selectedTabFont;

        private static readonly Dictionary<string, int> CategoryIndex = new Dictionary<string, int>
        {
            { "Settings", 0 },
            { "Items", 1 },
            { "Masks", 2 },
            { "Bottles", 3 },
            { "Songs", 4 },
            { "Rupees", 5 },
            { "Maps/Others", 6 }
        };

        public static void ConfigureCategoryTabs(TabControl tabControl)
        {
            UiTheme.ApplyTabControlTheme(tabControl);
            _imageList = BuildImageList();

            tabControl.ImageList = _imageList;
            tabControl.DrawMode = TabDrawMode.OwnerDrawFixed;
            tabControl.SizeMode = TabSizeMode.Fixed;
            tabControl.ItemSize = new Size(106, 30);
            tabControl.Padding = new Point(12, 4);
            tabControl.DrawItem -= DrawCategoryTab;
            tabControl.DrawItem += DrawCategoryTab;
            tabControl.SelectedIndexChanged -= TabControl_SelectedIndexChanged;
            tabControl.SelectedIndexChanged += TabControl_SelectedIndexChanged;

            foreach (TabPage page in tabControl.TabPages)
            {
                if (CategoryIndex.TryGetValue(page.Text, out int index))
                {
                    page.ImageIndex = index;
                    page.ImageKey = null;
                }
            }
        }

        public static void ConfigureTextTabs(TabControl tabControl)
        {
            UiTheme.ApplyTabControlTheme(tabControl);
            tabControl.ImageList = null;
            tabControl.DrawMode = TabDrawMode.OwnerDrawFixed;
            tabControl.SizeMode = TabSizeMode.Fixed;
            tabControl.ItemSize = MeasureTextTabItemSize(tabControl);
            tabControl.Padding = new Point(10, 4);
            tabControl.DrawItem -= DrawCategoryTab;
            tabControl.DrawItem -= DrawTextTab;
            tabControl.DrawItem += DrawTextTab;
            tabControl.SelectedIndexChanged -= TabControl_SelectedIndexChanged;
            tabControl.SelectedIndexChanged += TabControl_SelectedIndexChanged;

            foreach (TabPage page in tabControl.TabPages)
            {
                page.ImageIndex = -1;
                page.ImageKey = null;
            }

            if (tabControl.TabCount > 0)
            {
                tabControl.Invalidate(new Rectangle(0, 0, tabControl.Width, tabControl.GetTabRect(0).Bottom));
            }
        }

        private static Size MeasureTextTabItemSize(TabControl tabControl)
        {
            const int horizontalPadding = 24;
            const int tabHeight = 30;
            int maxWidth = 52;

            if (tabControl.TabCount == 0)
            {
                return new Size(maxWidth, tabHeight);
            }

            using (Font selectedFont = new Font(tabControl.Font, FontStyle.Bold))
            {
                foreach (TabPage page in tabControl.TabPages)
                {
                    Size normalSize = TextRenderer.MeasureText(
                        page.Text,
                        tabControl.Font,
                        Size.Empty,
                        TextFormatFlags.SingleLine | TextFormatFlags.NoPadding);
                    Size selectedSize = TextRenderer.MeasureText(
                        page.Text,
                        selectedFont,
                        Size.Empty,
                        TextFormatFlags.SingleLine | TextFormatFlags.NoPadding);
                    maxWidth = Math.Max(maxWidth, Math.Max(normalSize.Width, selectedSize.Width) + horizontalPadding);
                }
            }

            return new Size(maxWidth, tabHeight);
        }

        private static void DrawTextTab(object sender, DrawItemEventArgs e)
        {
            ThemePalette theme = UiTheme.Current;
            TabControl tabs = (TabControl)sender;
            TabPage page = tabs.TabPages[e.Index];
            bool selected = (e.State & DrawItemState.Selected) == DrawItemState.Selected;
            bool isLastTab = e.Index == tabs.TabCount - 1;

            Color backColor = selected ? theme.TabSelectedBackColor : theme.TabUnselectedBackColor;
            Color textColor = selected ? theme.TabSelectedAccentColor : theme.TabForeColor;

            using (SolidBrush backBrush = new SolidBrush(backColor))
            {
                e.Graphics.FillRectangle(backBrush, e.Bounds);
            }

            if (!selected)
            {
                using (Pen borderPen = new Pen(theme.BorderColor))
                {
                    Rectangle border = e.Bounds;
                    border.Width -= 1;
                    border.Height -= 1;
                    e.Graphics.DrawRectangle(borderPen, border);
                }
            }

            Font drawFont = selected ? GetSelectedTabFont(tabs.Font) : tabs.Font;
            TextRenderer.DrawText(
                e.Graphics,
                page.Text,
                drawFont,
                new Rectangle(e.Bounds.X + 10, e.Bounds.Y, e.Bounds.Width - 14, e.Bounds.Height),
                textColor,
                TextFormatFlags.Left | TextFormatFlags.VerticalCenter | TextFormatFlags.NoPrefix);

            if (isLastTab)
            {
                int stripHeight = tabs.GetTabRect(0).Bottom;
                int gapLeft = e.Bounds.Right;
                int gapWidth = tabs.ClientSize.Width - gapLeft;
                if (gapWidth > 0)
                {
                    using (SolidBrush stripBrush = new SolidBrush(theme.TabStripBackColor))
                    {
                        e.Graphics.FillRectangle(stripBrush, gapLeft, 0, gapWidth, stripHeight);
                    }
                }
            }
        }

        private static void TabControl_SelectedIndexChanged(object sender, EventArgs e)
        {
            TabControl tabs = (TabControl)sender;
            if (tabs.TabCount > 0)
            {
                tabs.Invalidate(new Rectangle(0, 0, tabs.Width, tabs.GetTabRect(0).Bottom));
            }
        }

        private static void DrawCategoryTab(object sender, DrawItemEventArgs e)
        {
            ThemePalette theme = UiTheme.Current;
            TabControl tabs = (TabControl)sender;
            TabPage page = tabs.TabPages[e.Index];
            bool selected = (e.State & DrawItemState.Selected) == DrawItemState.Selected;
            bool isLastTab = e.Index == tabs.TabCount - 1;

            Color backColor = selected ? theme.TabSelectedBackColor : theme.TabUnselectedBackColor;
            Color textColor = selected ? theme.TabSelectedAccentColor : theme.TabForeColor;
            Color iconColor = selected ? theme.TabSelectedAccentColor : theme.TabIconColor;

            using (SolidBrush backBrush = new SolidBrush(backColor))
            {
                e.Graphics.FillRectangle(backBrush, e.Bounds);
            }

            if (!selected)
            {
                using (Pen borderPen = new Pen(theme.BorderColor))
                {
                    Rectangle border = e.Bounds;
                    border.Width -= 1;
                    border.Height -= 1;
                    e.Graphics.DrawRectangle(borderPen, border);
                }
            }

            int iconSize = 16;
            int iconX = e.Bounds.X + 8;
            int iconY = e.Bounds.Y + (e.Bounds.Height - iconSize) / 2;
            DrawCategoryGlyph(e.Graphics, page.Text, iconX, iconY, iconSize, iconColor);

            Font drawFont = selected ? GetSelectedTabFont(tabs.Font) : tabs.Font;
            TextRenderer.DrawText(
                e.Graphics,
                page.Text,
                drawFont,
                new Rectangle(iconX + iconSize + 6, e.Bounds.Y, e.Bounds.Width - iconSize - 14, e.Bounds.Height),
                textColor,
                TextFormatFlags.Left | TextFormatFlags.VerticalCenter | TextFormatFlags.EndEllipsis);

            if (isLastTab)
            {
                int stripHeight = tabs.GetTabRect(0).Bottom;
                int gapLeft = e.Bounds.Right;
                int gapWidth = tabs.ClientSize.Width - gapLeft;
                if (gapWidth > 0)
                {
                    using (SolidBrush stripBrush = new SolidBrush(theme.TabStripBackColor))
                    {
                        e.Graphics.FillRectangle(stripBrush, gapLeft, 0, gapWidth, stripHeight);
                    }
                }
            }
        }

        private static Font GetSelectedTabFont(Font baseFont)
        {
            if (_selectedTabFont == null || _selectedTabFont.FontFamily.Name != baseFont.FontFamily.Name)
            {
                _selectedTabFont?.Dispose();
                _selectedTabFont = new Font(baseFont, FontStyle.Bold);
            }

            return _selectedTabFont;
        }

        private static ImageList BuildImageList()
        {
            ThemePalette theme = UiTheme.Current;
            ImageList list = new ImageList
            {
                ImageSize = new Size(16, 16),
                ColorDepth = ColorDepth.Depth32Bit
            };

            list.Images.Add(DrawIconBitmap("Settings", theme.TabIconColor));
            list.Images.Add(DrawIconBitmap("Items", theme.TabIconColor));
            list.Images.Add(DrawIconBitmap("Masks", theme.TabIconColor));
            list.Images.Add(DrawIconBitmap("Bottles", theme.TabIconColor));
            list.Images.Add(DrawIconBitmap("Songs", theme.TabIconColor));
            list.Images.Add(DrawIconBitmap("Rupees", theme.TabIconColor));
            list.Images.Add(DrawIconBitmap("Maps/Others", theme.TabIconColor));

            return list;
        }

        private static Bitmap DrawIconBitmap(string category, Color color)
        {
            Bitmap bitmap = new Bitmap(16, 16);
            using (Graphics g = Graphics.FromImage(bitmap))
            {
                g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
                g.Clear(Color.Transparent);
                DrawCategoryGlyph(g, category, 0, 0, 16, color);
            }

            return bitmap;
        }

        private static void DrawCategoryGlyph(Graphics g, string category, int x, int y, int size, Color color)
        {
            using (Pen pen = new Pen(color, 1.6f))
            {
                pen.StartCap = System.Drawing.Drawing2D.LineCap.Round;
                pen.EndCap = System.Drawing.Drawing2D.LineCap.Round;

                switch (category)
                {
                    case "Settings":
                        g.DrawEllipse(pen, x + 4, y + 4, size - 8, size - 8);
                        g.DrawLine(pen, x + 8, y + 2, x + 8, y + 5);
                        g.DrawLine(pen, x + 8, y + size - 5, x + 8, y + size - 2);
                        g.DrawLine(pen, x + 2, y + 8, x + 5, y + 8);
                        g.DrawLine(pen, x + size - 5, y + 8, x + size - 2, y + 8);
                        break;
                    case "Items":
                        g.DrawRectangle(pen, x + 2, y + 4, size - 5, size - 7);
                        g.DrawLine(pen, x + 2, y + 6, x + size - 3, y + 6);
                        g.DrawArc(pen, x + 4, y + 1, size - 9, 5, 0, 180);
                        break;
                    case "Masks":
                        g.DrawEllipse(pen, x + 2, y + 3, size - 5, size - 6);
                        g.DrawLine(pen, x + 5, y + 7, x + 7, y + 9);
                        g.DrawLine(pen, x + size - 7, y + 7, x + size - 5, y + 9);
                        break;
                    case "Bottles":
                        g.DrawRectangle(pen, x + 5, y + 2, 5, 3);
                        g.DrawRectangle(pen, x + 3, y + 5, size - 7, size - 7);
                        break;
                    case "Songs":
                        g.DrawEllipse(pen, x + 2, y + 9, 3, 3);
                        g.DrawEllipse(pen, x + 7, y + 6, 3, 3);
                        g.DrawLine(pen, x + 5, y + 10, x + 5, y + 3);
                        g.DrawLine(pen, x + 10, y + 7, x + 10, y + 2);
                        break;
                    case "Rupees":
                        {
                            Point[] gem = new[]
                            {
                                new Point(x + 8, y + 2),
                                new Point(x + size - 3, y + 7),
                                new Point(x + 8, y + size - 2),
                                new Point(x + 3, y + 7)
                            };
                            g.DrawPolygon(pen, gem);
                        }
                        break;
                    case "Maps/Others":
                        g.DrawRectangle(pen, x + 2, y + 3, size - 5, size - 6);
                        g.DrawLine(pen, x + size / 2, y + 3, x + size / 2, y + size - 3);
                        g.DrawLine(pen, x + 2, y + size / 2, x + size - 3, y + size / 2);
                        g.DrawLine(pen, x + 4, y + 5, x + size - 5, y + 5);
                        break;
                    default:
                        g.DrawRectangle(pen, x + 2, y + 3, size - 5, size - 6);
                        g.DrawLine(pen, x + 2, y + 3, x + size - 3, y + size - 3);
                        break;
                }
            }
        }
    }
}
