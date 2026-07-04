using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal partial class PlandoStrandedDialog : Form
    {
        public string SelectedLocation { get; private set; }

        private PlandoStrandedDialog(string item, IEnumerable<string> locations)
        {
            InitializeComponent();
            promptLabel.Text = item + " is plando'd away from its own location and is not placed elsewhere. Choose a location that should give it:";

            foreach (string location in locations)
            {
                _locationCombo.Items.Add(location);
            }

            if (_locationCombo.Items.Count > 0)
            {
                _locationCombo.SelectedIndex = 0;
            }

            UiTheme.ApplyToForm(this);
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
