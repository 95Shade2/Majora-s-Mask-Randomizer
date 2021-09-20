using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
    public partial class CutscenesSelector : Form
    {
        private Dictionary<int, CheckBox> checkboxes;

        public Main_Window father;
        public bool showing;

        public CutscenesSelector()
        {
            InitializeComponent();
        }

        private void Update_Checkboxes(Control.ControlCollection controls)
        {
            CheckBox check;
            string name;

            if (controls.Count == 0)
            {
                return;
            }

            foreach (Control control in controls)
            {
                if (control is CheckBox)
                {
                    check = ((CheckBox)control);
                    name = check.Name;

                    //if there is data for the cutscene
                    if (father.Cutscenes.ContainsKey(name))
                    {
                        //set it to what was stored
                        check.Checked = father.Cutscenes[name];
                    }
                    else
                    {
                        //otherwise, set it to false by default
                        ((CheckBox)control).Checked = false;
                    }
                }
                //get the sub controls in the form
                else
                {
                    Update_Checkboxes(control.Controls);
                }
            }
        }

        private void CutscenesSelector_Load(object sender, EventArgs e)
        {
            showing = true;
            
            Update_Checkboxes(this.Controls);
        }

        private void CutscenesSelector_FormClosing(object sender, FormClosingEventArgs e)
        {
            father.cs = new CutscenesSelector();
            showing = false;
        }

        private void ClockTown_NorthClockTownIntro_CheckedChanged(object sender, EventArgs e)
        {
            string name = ((CheckBox)sender).Name;

            father.Cutscenes[name] = ((CheckBox)sender).Checked;
        }

        /*private void Update_Checkboxes()
        {
            Dictionary<string, bool>.KeyCollection keys = father.Cutscenes.Keys;
            string name;
            CheckBox check;

            for (int k = 0; k < keys.Count; k++)
            {
                name = keys.ElementAt(k);
                check = (CheckBox)(this.Controls.Find(name, true))[0];
                check.Checked = father.Cutscenes[name];
            }
        }*/
    }
}
