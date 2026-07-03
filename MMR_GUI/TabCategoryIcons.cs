using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class TabCategoryIcons
    {
        private static readonly Color IconColor = Color.FromArgb(55, 55, 60);
        private static readonly Color SelectedIconColor = Color.FromArgb(98, 0, 234);
        private static readonly Color SelectedTextColor = Color.FromArgb(98, 0, 234);

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
            _imageList = BuildImageList();

            tabControl.ImageList = _imageList;
            tabControl.DrawMode = TabDrawMode.OwnerDrawFixed;
            tabControl.SizeMode = TabSizeMode.Fixed;
            tabControl.ItemSize = new Size(106, 30);
            tabControl.Padding = new Point(12, 4);
            tabControl.DrawItem -= DrawCategoryTab;
            tabControl.DrawItem += DrawCategoryTab;

            foreach (TabPage page in tabControl.TabPages)
            {
                if (CategoryIndex.TryGetValue(page.Text, out int index))
                {
                    page.ImageIndex = index;
                    page.ImageKey = null;
                }
            }
        }

        private static void DrawCategoryTab(object sender, DrawItemEventArgs e)
        {
            TabControl tabs = (TabControl)sender;
            TabPage page = tabs.TabPages[e.Index];
            bool selected = (e.State & DrawItemState.Selected) == DrawItemState.Selected;

            Color backColor = selected ? SystemColors.Window : tabs.BackColor;
            Color textColor = selected ? SelectedTextColor : SystemColors.ControlText;
            Color iconColor = selected ? SelectedIconColor : IconColor;

            using (SolidBrush backBrush = new SolidBrush(backColor))
            {
                e.Graphics.FillRectangle(backBrush, e.Bounds);
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
            ImageList list = new ImageList
            {
                ImageSize = new Size(16, 16),
                ColorDepth = ColorDepth.Depth32Bit
            };

            list.Images.Add(DrawIconBitmap("Settings", IconColor));
            list.Images.Add(DrawIconBitmap("Items", IconColor));
            list.Images.Add(DrawIconBitmap("Masks", IconColor));
            list.Images.Add(DrawIconBitmap("Bottles", IconColor));
            list.Images.Add(DrawIconBitmap("Songs", IconColor));
            list.Images.Add(DrawIconBitmap("Rupees", IconColor));
            list.Images.Add(DrawIconBitmap("Maps/Others", IconColor));

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
