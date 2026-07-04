using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal sealed class PlandoStrandedDialog : Form
    {
        private readonly ComboBox _locationCombo;
        public string SelectedLocation { get; private set; }

        private PlandoStrandedDialog(string item, IEnumerable<string> locations)
        {
            Text = "Place Stranded Item";
            FormBorderStyle = FormBorderStyle.FixedDialog;
            MaximizeBox = false;
            MinimizeBox = false;
            StartPosition = FormStartPosition.CenterParent;
            AutoSize = true;
            AutoSizeMode = AutoSizeMode.GrowAndShrink;
            Padding = new Padding(12);
            Font = UiTheme.Current.BaseFont;

            Label prompt = new Label
            {
                AutoSize = true,
                MaximumSize = new Size(360, 0),
                Text = item + " is plando'd away from its own location and is not placed elsewhere. Choose a location that should give it:",
                Margin = new Padding(0, 0, 0, 8)
            };

            _locationCombo = new ComboBox
            {
                DropDownStyle = ComboBoxStyle.DropDownList,
                Width = 320,
                Margin = new Padding(0, 0, 0, 12)
            };

            foreach (string location in locations)
            {
                _locationCombo.Items.Add(location);
            }

            if (_locationCombo.Items.Count > 0)
            {
                _locationCombo.SelectedIndex = 0;
            }

            FlowLayoutPanel buttons = new FlowLayoutPanel
            {
                AutoSize = true,
                FlowDirection = FlowDirection.RightToLeft,
                WrapContents = false,
                Dock = DockStyle.Fill
            };

            Button okButton = new Button
            {
                Text = "Add Plando",
                DialogResult = DialogResult.OK,
                AutoSize = true,
                Margin = new Padding(8, 0, 0, 0)
            };
            Button cancelButton = new Button
            {
                Text = "Cancel",
                DialogResult = DialogResult.Cancel,
                AutoSize = true
            };

            buttons.Controls.Add(okButton);
            buttons.Controls.Add(cancelButton);

            TableLayoutPanel layout = new TableLayoutPanel
            {
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                ColumnCount = 1,
                RowCount = 3,
                Dock = DockStyle.Fill
            };
            layout.Controls.Add(prompt, 0, 0);
            layout.Controls.Add(_locationCombo, 0, 1);
            layout.Controls.Add(buttons, 0, 2);

            Controls.Add(layout);
            AcceptButton = okButton;
            CancelButton = cancelButton;
        }

        public static bool TryResolve(
            IWin32Window owner,
            string item,
            string[] allItems,
            Dictionary<string, string> plandoItems,
            out string location)
        {
            location = "";

            List<string> eligible = GetEligibleLocations(item, allItems, plandoItems);
            if (eligible.Count == 0)
            {
                MessageBox.Show(
                    owner,
                    "No open locations are available to place " + item + ". Remove or change an existing plando entry first.",
                    "Resolve Stranded Item",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Warning);
                return false;
            }

            using (PlandoStrandedDialog dialog = new PlandoStrandedDialog(item, eligible))
            {
                if (dialog.ShowDialog(owner) != DialogResult.OK || dialog._locationCombo.SelectedIndex < 0)
                {
                    return false;
                }

                location = dialog._locationCombo.SelectedItem.ToString();
                return location != "";
            }
        }

        private static List<string> GetEligibleLocations(
            string item,
            string[] allItems,
            Dictionary<string, string> plandoItems)
        {
            List<string> eligible = new List<string>();

            if (allItems == null)
            {
                return eligible;
            }

            for (int i = 0; i < allItems.Length; i++)
            {
                string candidate = allItems[i];
                if (candidate == item)
                {
                    continue;
                }

                if (plandoItems != null && plandoItems.ContainsKey(candidate))
                {
                    continue;
                }

                eligible.Add(candidate);
            }

            return eligible.OrderBy(name => name).ToList();
        }
    }
}
