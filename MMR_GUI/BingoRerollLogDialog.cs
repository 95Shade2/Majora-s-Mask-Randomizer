using System;
using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    internal sealed class BingoRerollLogDialog : Form
    {
        private readonly TextBox _logBox;

        public BingoRerollLogDialog(BingoCard card, IWin32Window owner)
        {
            Text = "Bingo Reroll Log";
            StartPosition = FormStartPosition.CenterParent;
            MinimumSize = new Size(640, 420);
            ClientSize = new Size(720, 480);
            Font = UiTheme.Current.BaseFont;

            _logBox = new TextBox
            {
                Multiline = true,
                ReadOnly = true,
                ScrollBars = ScrollBars.Both,
                WordWrap = true,
                Dock = DockStyle.Fill,
                Font = new Font("Consolas", 9F),
                Text = BingoRerollLog.Format(card)
            };

            FlowLayoutPanel buttons = new FlowLayoutPanel
            {
                Dock = DockStyle.Bottom,
                AutoSize = true,
                FlowDirection = FlowDirection.RightToLeft,
                Padding = new Padding(0, 8, 0, 0)
            };

            Button closeButton = new Button
            {
                Text = "Close",
                AutoSize = true,
                DialogResult = DialogResult.OK
            };
            Button copyButton = new Button
            {
                Text = "Copy log",
                AutoSize = true,
                Margin = new Padding(0, 0, 8, 0)
            };
            copyButton.Click += (s, e) => Clipboard.SetText(_logBox.Text);

            buttons.Controls.Add(closeButton);
            buttons.Controls.Add(copyButton);

            Controls.Add(buttons);
            Controls.Add(_logBox);
            AcceptButton = closeButton;

            Form ownerForm = owner as Form;
            if (ownerForm != null)
            {
                Icon = ownerForm.Icon;
            }
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
    }
}
