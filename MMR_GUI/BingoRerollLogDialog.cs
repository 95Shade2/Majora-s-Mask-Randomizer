using System;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal partial class BingoRerollLogDialog : Form
    {
        private readonly BingoCard _card;

        public BingoRerollLogDialog(BingoCard card, IWin32Window owner)
        {
            _card = card;
            InitializeComponent();

            Form ownerForm = owner as Form;
            if (ownerForm != null)
            {
                Icon = ownerForm.Icon;
            }

            _logBox.Text = BingoRerollLog.Format(card);
            UiTheme.ApplyToForm(this);
        }

        public static void ShowForCard(BingoCard card, IWin32Window owner)
        {
            if (card == null)
            {
                return;
            }

            using (BingoRerollLogDialog dialog = new BingoRerollLogDialog(card, owner))
            {
                dialog.ShowDialog(owner);
            }
        }

        private void copyButton_Click(object sender, EventArgs e)
        {
            Clipboard.SetText(_logBox.Text);
        }
    }
}
