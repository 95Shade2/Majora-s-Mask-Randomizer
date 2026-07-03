using System;
using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal static class PoolListView
    {
        private static readonly Color EvenRow = Color.White;
        private static readonly Color OddRow = Color.FromArgb(242, 245, 250);

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
            list.DrawColumnHeader += DrawColumnHeader;
            list.DrawItem += DrawItem;
            list.DrawSubItem += DrawSubItem;
            list.Resize += (sender, args) =>
            {
                if (list.Columns.Count > 0)
                {
                    list.Columns[0].Width = Math.Max(80, list.ClientSize.Width - 4);
                }
            };
            UiTheme.EnableDoubleBuffer(list);
            return list;
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

        private static void DrawColumnHeader(object sender, DrawListViewColumnHeaderEventArgs e)
        {
            e.DrawDefault = true;
        }

        private static void DrawItem(object sender, DrawListViewItemEventArgs e)
        {
            e.DrawDefault = false;
        }

        private static void DrawSubItem(object sender, DrawListViewSubItemEventArgs e)
        {
            Color back = e.ItemIndex % 2 == 0 ? EvenRow : OddRow;
            using (SolidBrush brush = new SolidBrush(back))
            {
                e.Graphics.FillRectangle(brush, e.Bounds);
            }

            TextRenderer.DrawText(
                e.Graphics,
                e.SubItem.Text,
                e.Item.ListView.Font,
                e.Bounds,
                SystemColors.ControlText,
                TextFormatFlags.Left | TextFormatFlags.VerticalCenter | TextFormatFlags.EndEllipsis);
        }
    }
}
