using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;

namespace Majora_s_Mask_Randomizer_GUI
{
    class Item
    {
        CheckBox Item_CheckBox;
        ComboBox Item_Pool;
        ComboBox Item_Gives;

        public Item(CheckBox box, ComboBox pool, ComboBox gives)
        {
            Item_CheckBox = box;
            Item_Pool = pool;
            Item_Gives = gives;
        }

        public ComboBox Get_Pool()
        {
            return Item_Pool;
        }

        public ComboBox Get_Gives()
        {
            return Item_Gives;
        }

        public CheckBox Get_Checkbox()
        {
            return Item_CheckBox;
        }
    }
}
