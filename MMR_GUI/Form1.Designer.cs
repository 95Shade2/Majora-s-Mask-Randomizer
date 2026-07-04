using System;
using System.Drawing;
using System.Windows.Forms;

namespace Majora_s_Mask_Randomizer_GUI
{
        partial class Main_Window
        {
                /// <summary>
                /// Required designer variable.
                /// </summary>
                private System.ComponentModel.IContainer components = null;

                /// <summary>
                /// Clean up any resources being used.
                /// </summary>
                /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
                protected override void Dispose(bool disposing)
                {
                        if (disposing && (components != null))
                        {
                                components.Dispose();
                        }
                        base.Dispose(disposing);
                }

                                #region Windows Form Designer generated code

                private void InitializeComponent()
                {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main_Window));
            this.Pools_Label = new System.Windows.Forms.Label();
            this.Pool_Tabs = new Majora_s_Mask_Randomizer_GUI.ThemedTabControl();
            this.Create_Pool_Textbox = new System.Windows.Forms.TextBox();
            this.Create_Pool_Label = new System.Windows.Forms.Label();
            this.Create_Pool_Button = new System.Windows.Forms.Button();
            this.Remove_Pool_Combobox = new System.Windows.Forms.ComboBox();
            this.Remove_Pool_Button = new System.Windows.Forms.Button();
            this.Remove_Pool_Label = new System.Windows.Forms.Label();
            this.Change_Pool_Name_Label = new System.Windows.Forms.Label();
            this.Change_Pool_Name_Combobox = new System.Windows.Forms.ComboBox();
            this.Change_Pool_Name_Textbox = new System.Windows.Forms.TextBox();
            this.Change_Pool_Name_Button = new System.Windows.Forms.Button();
            this.Randomize_Button = new System.Windows.Forms.Button();
            this.Save_Preset_Button = new System.Windows.Forms.Button();
            this.Load_Preset_Button = new System.Windows.Forms.Button();
            this.Open_Base_Rom_Button = new System.Windows.Forms.Button();
            this.Base_Rom_Label = new System.Windows.Forms.Label();
            this.Open_Base_Rom_Dialog = new System.Windows.Forms.OpenFileDialog();
            this.Save_Presets_Dialog = new System.Windows.Forms.SaveFileDialog();
            this.Presets_Combobox = new System.Windows.Forms.ComboBox();
            this.Presets_Label = new System.Windows.Forms.Label();
            this.Seed_Textbox = new System.Windows.Forms.TextBox();
            this.Seed_Label = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.Logic_Combobox = new System.Windows.Forms.ComboBox();
            this.Logic_Label = new System.Windows.Forms.Label();
            this.Scrub_Sells_Beans_Tooltip = new System.Windows.Forms.ToolTip();
            this.Tunic_Label = new System.Windows.Forms.Label();
            this.Tunic_ColorDialog = new System.Windows.Forms.ColorDialog();
            this.Tunic_Button = new System.Windows.Forms.Button();
            this.Items_Tab = new Majora_s_Mask_Randomizer_GUI.ThemedTabControl();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.settingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createWadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.swampScrubSalesBeansToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.playAsKafeiToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeCutscenesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gCHudToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.respawnHPsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.edibleMirrorShieldToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.keepRazorSwordOnSoTToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.oceanSpiderHouseAnyDayToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.respawnHCsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeScrubSalesmanAfterTradingToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pauseMenuColorsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.walletToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.logicToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cutscenesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.BlastMaskFrames_Num = new System.Windows.Forms.NumericUpDown();
            this.BlastMaskSeconds_Label = new System.Windows.Forms.Label();
            this.Targeting_Switch = new System.Windows.Forms.RadioButton();
            this.Targeting_Hold = new System.Windows.Forms.RadioButton();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.BlastMaskFrames_Num)).BeginInit();
            this.SuspendLayout();
            // Pools_Label

            // 

            this.Pools_Label.AutoSize = true;

            this.Pools_Label.Location = new System.Drawing.Point(584, 36);

            this.Pools_Label.Name = "Pools_Label";

            this.Pools_Label.Size = new System.Drawing.Size(56, 13);

            this.Pools_Label.TabIndex = 3;

            this.Pools_Label.Text = "Item Pools";

            //
            // Pool_Tabs

            // 

            this.Pool_Tabs.Location = new System.Drawing.Point(532, 52);

            this.Pool_Tabs.Name = "Pool_Tabs";

            this.Pool_Tabs.SelectedIndex = 0;

            this.Pool_Tabs.Size = new System.Drawing.Size(160, 401);

            this.Pool_Tabs.TabIndex = 4;

            //
            // Create_Pool_Textbox

            // 

            this.Create_Pool_Textbox.Location = new System.Drawing.Point(89, 29);

            this.Create_Pool_Textbox.Name = "Create_Pool_Textbox";

            this.Create_Pool_Textbox.Size = new System.Drawing.Size(100, 20);

            this.Create_Pool_Textbox.TabIndex = 7;

            //
            // Create_Pool_Label

            // 

            this.Create_Pool_Label.AutoSize = true;

            this.Create_Pool_Label.Location = new System.Drawing.Point(12, 32);

            this.Create_Pool_Label.Name = "Create_Pool_Label";

            this.Create_Pool_Label.Size = new System.Drawing.Size(62, 13);

            this.Create_Pool_Label.TabIndex = 8;

            this.Create_Pool_Label.Text = "Create Pool";

            //
            // Create_Pool_Button

            // 

            this.Create_Pool_Button.Location = new System.Drawing.Point(195, 27);

            this.Create_Pool_Button.Name = "Create_Pool_Button";

            this.Create_Pool_Button.Size = new System.Drawing.Size(75, 23);

            this.Create_Pool_Button.TabIndex = 9;

            this.Create_Pool_Button.Text = "Create";

            this.Create_Pool_Button.UseVisualStyleBackColor = true;

            this.Create_Pool_Button.Click += new System.EventHandler(this.Create_Pool_Button_Click);

            //
            // Remove_Pool_Combobox

            // 

            this.Remove_Pool_Combobox.FormattingEnabled = true;

            this.Remove_Pool_Combobox.Location = new System.Drawing.Point(353, 29);

            this.Remove_Pool_Combobox.Name = "Remove_Pool_Combobox";

            this.Remove_Pool_Combobox.Size = new System.Drawing.Size(100, 21);

            this.Remove_Pool_Combobox.TabIndex = 10;

            this.Remove_Pool_Combobox.SelectedIndexChanged += new System.EventHandler(this.Remove_Pool_Combobox_SelectedIndexChanged);

            //
            // Remove_Pool_Button

            // 

            this.Remove_Pool_Button.Location = new System.Drawing.Point(459, 27);

            this.Remove_Pool_Button.Name = "Remove_Pool_Button";

            this.Remove_Pool_Button.Size = new System.Drawing.Size(75, 23);

            this.Remove_Pool_Button.TabIndex = 11;

            this.Remove_Pool_Button.Text = "Remove";

            this.Remove_Pool_Button.UseVisualStyleBackColor = true;

            this.Remove_Pool_Button.Click += new System.EventHandler(this.Remove_Pool_Button_Click);

            //
            // Remove_Pool_Label

            // 

            this.Remove_Pool_Label.AutoSize = true;

            this.Remove_Pool_Label.Location = new System.Drawing.Point(276, 32);

            this.Remove_Pool_Label.Name = "Remove_Pool_Label";

            this.Remove_Pool_Label.Size = new System.Drawing.Size(71, 13);

            this.Remove_Pool_Label.TabIndex = 12;

            this.Remove_Pool_Label.Text = "Remove Pool";

            //
            // Change_Pool_Name_Label

            // 

            this.Change_Pool_Name_Label.AutoSize = true;

            this.Change_Pool_Name_Label.Location = new System.Drawing.Point(12, 58);

            this.Change_Pool_Name_Label.Name = "Change_Pool_Name_Label";

            this.Change_Pool_Name_Label.Size = new System.Drawing.Size(75, 13);

            this.Change_Pool_Name_Label.TabIndex = 13;

            this.Change_Pool_Name_Label.Text = "Change Name";

            //
            // Change_Pool_Name_Combobox

            // 

            this.Change_Pool_Name_Combobox.FormattingEnabled = true;

            this.Change_Pool_Name_Combobox.Location = new System.Drawing.Point(89, 55);

            this.Change_Pool_Name_Combobox.Name = "Change_Pool_Name_Combobox";

            this.Change_Pool_Name_Combobox.Size = new System.Drawing.Size(100, 21);

            this.Change_Pool_Name_Combobox.TabIndex = 14;

            //
            // Change_Pool_Name_Textbox

            // 

            this.Change_Pool_Name_Textbox.Location = new System.Drawing.Point(195, 56);

            this.Change_Pool_Name_Textbox.Name = "Change_Pool_Name_Textbox";

            this.Change_Pool_Name_Textbox.Size = new System.Drawing.Size(100, 20);

            this.Change_Pool_Name_Textbox.TabIndex = 15;

            //
            // Change_Pool_Name_Button

            // 

            this.Change_Pool_Name_Button.Location = new System.Drawing.Point(301, 55);

            this.Change_Pool_Name_Button.Name = "Change_Pool_Name_Button";

            this.Change_Pool_Name_Button.Size = new System.Drawing.Size(75, 23);

            this.Change_Pool_Name_Button.TabIndex = 16;

            this.Change_Pool_Name_Button.Text = "Change";

            this.Change_Pool_Name_Button.UseVisualStyleBackColor = true;

            this.Change_Pool_Name_Button.Click += new System.EventHandler(this.Change_Pool_Name_Button_Click);

            //
            // Randomize_Button

            // 

            this.Randomize_Button.Location = new System.Drawing.Point(433, 430);

            this.Randomize_Button.Name = "Randomize_Button";

            this.Randomize_Button.Size = new System.Drawing.Size(75, 23);

            this.Randomize_Button.TabIndex = 56;

            this.Randomize_Button.Text = "Randomize";

            this.Randomize_Button.UseVisualStyleBackColor = true;

            this.Randomize_Button.Click += new System.EventHandler(this.Randomize_Button_Click);

            this.Randomize_Button.MouseHover += new System.EventHandler(this.Randomize_Button_MouseHover);

            //
            // Save_Preset_Button

            // 

            this.Save_Preset_Button.Location = new System.Drawing.Point(433, 160);

            this.Save_Preset_Button.Name = "Save_Preset_Button";

            this.Save_Preset_Button.Size = new System.Drawing.Size(75, 23);

            this.Save_Preset_Button.TabIndex = 57;

            this.Save_Preset_Button.Text = "Save Preset";

            this.Save_Preset_Button.UseVisualStyleBackColor = true;

            this.Save_Preset_Button.Click += new System.EventHandler(this.Save_Preset_Button_Click);

            //
            // Load_Preset_Button

            // 

            this.Load_Preset_Button.Location = new System.Drawing.Point(433, 131);

            this.Load_Preset_Button.Name = "Load_Preset_Button";

            this.Load_Preset_Button.Size = new System.Drawing.Size(75, 23);

            this.Load_Preset_Button.TabIndex = 58;

            this.Load_Preset_Button.Text = "Load Preset";

            this.Load_Preset_Button.UseVisualStyleBackColor = true;

            this.Load_Preset_Button.Click += new System.EventHandler(this.Load_Preset_Button_Click);

            //
            // Open_Base_Rom_Button

            // 

            this.Open_Base_Rom_Button.Location = new System.Drawing.Point(433, 233);

            this.Open_Base_Rom_Button.Name = "Open_Base_Rom_Button";

            this.Open_Base_Rom_Button.Size = new System.Drawing.Size(75, 23);

            this.Open_Base_Rom_Button.TabIndex = 59;

            this.Open_Base_Rom_Button.Text = "Base Rom";

            this.Open_Base_Rom_Button.UseVisualStyleBackColor = true;

            this.Open_Base_Rom_Button.Click += new System.EventHandler(this.button1_Click);

            //
            // Base_Rom_Label

            // 

            this.Base_Rom_Label.AutoSize = true;

            this.Base_Rom_Label.Location = new System.Drawing.Point(413, 265);

            this.Base_Rom_Label.Name = "Base_Rom_Label";

            this.Base_Rom_Label.Size = new System.Drawing.Size(0, 13);

            this.Base_Rom_Label.TabIndex = 60;

            //
            // Open_Base_Rom_Dialog

            // 

            this.Open_Base_Rom_Dialog.FileName = "openFileDialog1";

            this.Open_Base_Rom_Dialog.FileOk += new System.ComponentModel.CancelEventHandler(this.Open_Base_Rom_Dialog_FileOk);

            //
            // Save_Presets_Dialog

            // 

            this.Save_Presets_Dialog.DefaultExt = "ini";

            this.Save_Presets_Dialog.FileName = "preset";

            this.Save_Presets_Dialog.RestoreDirectory = true;

            this.Save_Presets_Dialog.FileOk += new System.ComponentModel.CancelEventHandler(this.Save_Presets_Dialog_FileOk);

            //
            // Presets_Combobox

            // 

            this.Presets_Combobox.FormattingEnabled = true;

            this.Presets_Combobox.Location = new System.Drawing.Point(413, 104);

            this.Presets_Combobox.Name = "Presets_Combobox";

            this.Presets_Combobox.Size = new System.Drawing.Size(113, 21);

            this.Presets_Combobox.TabIndex = 61;

            //
            // Presets_Label

            // 

            this.Presets_Label.AutoSize = true;

            this.Presets_Label.Location = new System.Drawing.Point(446, 88);

            this.Presets_Label.Name = "Presets_Label";

            this.Presets_Label.Size = new System.Drawing.Size(42, 13);

            this.Presets_Label.TabIndex = 62;

            this.Presets_Label.Text = "Presets";

            //
            // Seed_Textbox

            // 

            this.Seed_Textbox.Location = new System.Drawing.Point(420, 404);

            this.Seed_Textbox.Name = "Seed_Textbox";

            this.Seed_Textbox.Size = new System.Drawing.Size(100, 20);

            this.Seed_Textbox.TabIndex = 63;

            //
            // Seed_Label

            // 

            this.Seed_Label.AutoSize = true;

            this.Seed_Label.Location = new System.Drawing.Point(454, 376);

            this.Seed_Label.Name = "Seed_Label";

            this.Seed_Label.Size = new System.Drawing.Size(32, 13);

            this.Seed_Label.TabIndex = 64;

            this.Seed_Label.Text = "Seed";

            //
            // label20

            // 

            this.label20.AutoSize = true;

            this.label20.Font = new System.Drawing.Font("Microsoft Sans Serif", 6.7F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));

            this.label20.Location = new System.Drawing.Point(418, 389);

            this.label20.Name = "label20";

            this.label20.Size = new System.Drawing.Size(107, 12);

            this.label20.TabIndex = 65;

            this.label20.Text = "(Leave blank for random)";

            //
            // Logic_Combobox

            // 

            this.Logic_Combobox.FormattingEnabled = true;

            this.Logic_Combobox.Location = new System.Drawing.Point(414, 206);

            this.Logic_Combobox.Name = "Logic_Combobox";

            this.Logic_Combobox.Size = new System.Drawing.Size(112, 21);

            this.Logic_Combobox.TabIndex = 67;

            //
            // Logic_Label

            // 

            this.Logic_Label.AutoSize = true;

            this.Logic_Label.Location = new System.Drawing.Point(451, 188);

            this.Logic_Label.Name = "Logic_Label";

            this.Logic_Label.Size = new System.Drawing.Size(33, 13);

            this.Logic_Label.TabIndex = 68;

            this.Logic_Label.Text = "Logic";

            //
            // Scrub_Sells_Beans_Tooltip

            // 

            this.Scrub_Sells_Beans_Tooltip.AutoPopDelay = 10000;

            this.Scrub_Sells_Beans_Tooltip.InitialDelay = 500;

            this.Scrub_Sells_Beans_Tooltip.ReshowDelay = 100;

            //
            // Tunic_Label

            // 

            this.Tunic_Label.AutoSize = true;

            this.Tunic_Label.Location = new System.Drawing.Point(596, 26);

            this.Tunic_Label.Name = "Tunic_Label";

            this.Tunic_Label.Size = new System.Drawing.Size(66, 13);

            this.Tunic_Label.TabIndex = 71;

            this.Tunic_Label.Text = "Tunic Colors";

            this.Tunic_Label.Visible = false;

            //
            // Tunic_ColorDialog

            // 

            this.Tunic_ColorDialog.Color = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(105)))), ((int)(((byte)(27)))));

            //
            // Tunic_Button

            // 

            this.Tunic_Button.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(105)))), ((int)(((byte)(27)))));

            this.Tunic_Button.Location = new System.Drawing.Point(666, 22);

            this.Tunic_Button.Name = "Tunic_Button";

            this.Tunic_Button.Size = new System.Drawing.Size(22, 23);

            this.Tunic_Button.TabIndex = 72;

            this.Tunic_Button.UseVisualStyleBackColor = false;

            this.Tunic_Button.Visible = false;

            this.Tunic_Button.Click += new System.EventHandler(this.Tunic_Button_Click);

            //
            // Items_Tab

            // 

            this.Items_Tab.Location = new System.Drawing.Point(2, 88);

            this.Items_Tab.Name = "Items_Tab";

            this.Items_Tab.SelectedIndex = 0;

            this.Items_Tab.Size = new System.Drawing.Size(410, 369);

            this.Items_Tab.TabIndex = 91;

            //
            // menuStrip1

            // 

            this.menuStrip1.BackColor = System.Drawing.SystemColors.ControlLightLight;

            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {

            this.settingsToolStripMenuItem,

            this.pauseMenuColorsToolStripMenuItem,

            this.walletToolStripMenuItem,

            this.logicToolStripMenuItem,

            this.cutscenesToolStripMenuItem});

            this.menuStrip1.Location = new System.Drawing.Point(0, 0);

            this.menuStrip1.Name = "menuStrip1";

            this.menuStrip1.Size = new System.Drawing.Size(700, 24);

            this.menuStrip1.TabIndex = 93;

            this.menuStrip1.Text = "menuStrip1";

            //
            // settingsToolStripMenuItem

            // 

            this.settingsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {

            this.createWadToolStripMenuItem,

            this.swampScrubSalesBeansToolStripMenuItem,

            this.playAsKafeiToolStripMenuItem,

            this.removeCutscenesToolStripMenuItem,

            this.gCHudToolStripMenuItem,

            this.respawnHPsToolStripMenuItem,

            this.edibleMirrorShieldToolStripMenuItem,

            this.keepRazorSwordOnSoTToolStripMenuItem,

            this.oceanSpiderHouseAnyDayToolStripMenuItem,

            this.respawnHCsToolStripMenuItem,

            this.removeScrubSalesmanAfterTradingToolStripMenuItem});

            this.settingsToolStripMenuItem.Name = "settingsToolStripMenuItem";

            this.settingsToolStripMenuItem.Size = new System.Drawing.Size(61, 20);

            this.settingsToolStripMenuItem.Text = "Settings";

            this.settingsToolStripMenuItem.Click += new System.EventHandler(this.settingsToolStripMenuItem_Click);

            //
            // createWadToolStripMenuItem

            // 

            this.createWadToolStripMenuItem.Name = "createWadToolStripMenuItem";

            this.createWadToolStripMenuItem.Size = new System.Drawing.Size(271, 22);

            this.createWadToolStripMenuItem.Text = "Create Wad";

            this.createWadToolStripMenuItem.Click += new System.EventHandler(this.createWadToolStripMenuItem_Click);

            //
            // swampScrubSalesBeansToolStripMenuItem

            // 

            this.swampScrubSalesBeansToolStripMenuItem.Name = "swampScrubSalesBeansToolStripMenuItem";

            this.swampScrubSalesBeansToolStripMenuItem.Size = new System.Drawing.Size(271, 22);

            this.swampScrubSalesBeansToolStripMenuItem.Text = "Swamp Scrub Sales Beans";

            this.swampScrubSalesBeansToolStripMenuItem.Click += new System.EventHandler(this.createWadToolStripMenuItem_Click);

            //
            // playAsKafeiToolStripMenuItem

            // 

            this.playAsKafeiToolStripMenuItem.Name = "playAsKafeiToolStripMenuItem";

            this.playAsKafeiToolStripMenuItem.Size = new System.Drawing.Size(271, 22);

            this.playAsKafeiToolStripMenuItem.Text = "Play as Kafei";

            this.playAsKafeiToolStripMenuItem.Click += new System.EventHandler(this.createWadToolStripMenuItem_Click);

            //
            // removeCutscenesToolStripMenuItem

            // 

            this.removeCutscenesToolStripMenuItem.Name = "removeCutscenesToolStripMenuItem";

            this.removeCutscenesToolStripMenuItem.Size = new System.Drawing.Size(271, 22);

            this.removeCutscenesToolStripMenuItem.Text = "Remove Cutscenes";

            this.removeCutscenesToolStripMenuItem.Visible = false;

            this.removeCutscenesToolStripMenuItem.Click += new System.EventHandler(this.createWadToolStripMenuItem_Click);

            //
            // gCHudToolStripMenuItem

            // 

            this.gCHudToolStripMenuItem.Name = "gCHudToolStripMenuItem";

            this.gCHudToolStripMenuItem.Size = new System.Drawing.Size(271, 22);

            this.gCHudToolStripMenuItem.Text = "GC Hud";

            this.gCHudToolStripMenuItem.Click += new System.EventHandler(this.createWadToolStripMenuItem_Click);

            //
            // respawnHPsToolStripMenuItem

            // 

            this.respawnHPsToolStripMenuItem.Name = "respawnHPsToolStripMenuItem";

            this.respawnHPsToolStripMenuItem.Size = new System.Drawing.Size(271, 22);

            this.respawnHPsToolStripMenuItem.Text = "Respawn HPs";

            this.respawnHPsToolStripMenuItem.Click += new System.EventHandler(this.createWadToolStripMenuItem_Click);

            //
            // edibleMirrorShieldToolStripMenuItem

            // 

            this.edibleMirrorShieldToolStripMenuItem.Name = "edibleMirrorShieldToolStripMenuItem";

            this.edibleMirrorShieldToolStripMenuItem.Size = new System.Drawing.Size(271, 22);

            this.edibleMirrorShieldToolStripMenuItem.Text = "Edible Mirror Shield";

            this.edibleMirrorShieldToolStripMenuItem.Click += new System.EventHandler(this.createWadToolStripMenuItem_Click);

            //
            // keepRazorSwordOnSoTToolStripMenuItem

            // 

            this.keepRazorSwordOnSoTToolStripMenuItem.Name = "keepRazorSwordOnSoTToolStripMenuItem";

            this.keepRazorSwordOnSoTToolStripMenuItem.Size = new System.Drawing.Size(271, 22);

            this.keepRazorSwordOnSoTToolStripMenuItem.Text = "Keep Razor Sword on SoT";

            this.keepRazorSwordOnSoTToolStripMenuItem.Click += new System.EventHandler(this.createWadToolStripMenuItem_Click);

            //
            // oceanSpiderHouseAnyDayToolStripMenuItem

            // 

            this.oceanSpiderHouseAnyDayToolStripMenuItem.Name = "oceanSpiderHouseAnyDayToolStripMenuItem";

            this.oceanSpiderHouseAnyDayToolStripMenuItem.Size = new System.Drawing.Size(271, 22);

            this.oceanSpiderHouseAnyDayToolStripMenuItem.Text = "Ocean Spider House Any Day";

            this.oceanSpiderHouseAnyDayToolStripMenuItem.Click += new System.EventHandler(this.createWadToolStripMenuItem_Click);

            //
            // respawnHCsToolStripMenuItem

            // 

            this.respawnHCsToolStripMenuItem.Name = "respawnHCsToolStripMenuItem";

            this.respawnHCsToolStripMenuItem.Size = new System.Drawing.Size(271, 22);

            this.respawnHCsToolStripMenuItem.Text = "Respawn HCs";

            this.respawnHCsToolStripMenuItem.Click += new System.EventHandler(this.createWadToolStripMenuItem_Click);

            //
            // removeScrubSalesmanAfterTradingToolStripMenuItem

            // 

            this.removeScrubSalesmanAfterTradingToolStripMenuItem.Name = "removeScrubSalesmanAfterTradingToolStripMenuItem";

            this.removeScrubSalesmanAfterTradingToolStripMenuItem.Size = new System.Drawing.Size(271, 22);

            this.removeScrubSalesmanAfterTradingToolStripMenuItem.Text = "Remove Scrub Salesmen after trading";

            this.removeScrubSalesmanAfterTradingToolStripMenuItem.Click += new System.EventHandler(this.createWadToolStripMenuItem_Click);

            //
            // pauseMenuColorsToolStripMenuItem

            // 

            this.pauseMenuColorsToolStripMenuItem.Name = "pauseMenuColorsToolStripMenuItem";

            this.pauseMenuColorsToolStripMenuItem.Size = new System.Drawing.Size(53, 20);

            this.pauseMenuColorsToolStripMenuItem.Text = "Colors";

            this.pauseMenuColorsToolStripMenuItem.Click += new System.EventHandler(this.pauseMenuColorsToolStripMenuItem_Click);

            //
            // walletToolStripMenuItem

            // 

            this.walletToolStripMenuItem.Name = "walletToolStripMenuItem";

            this.walletToolStripMenuItem.Size = new System.Drawing.Size(57, 20);

            this.walletToolStripMenuItem.Text = "Wallets";

            this.walletToolStripMenuItem.Click += new System.EventHandler(this.walletToolStripMenuItem_Click);

            //
            // logicToolStripMenuItem

            // 

            this.logicToolStripMenuItem.Name = "logicToolStripMenuItem";

            this.logicToolStripMenuItem.Size = new System.Drawing.Size(82, 20);

            this.logicToolStripMenuItem.Text = "Logic Editor";

            this.logicToolStripMenuItem.Click += new System.EventHandler(this.logicToolStripMenuItem_Click);

            //
            // cutscenesToolStripMenuItem

            // 

            this.cutscenesToolStripMenuItem.Name = "cutscenesToolStripMenuItem";

            this.cutscenesToolStripMenuItem.Size = new System.Drawing.Size(73, 20);

            this.cutscenesToolStripMenuItem.Text = "Cutscenes";

            this.cutscenesToolStripMenuItem.Click += new System.EventHandler(this.cutscenesToolStripMenuItem_Click);

            //
            // label5

            // 

            this.label5.AutoSize = true;

            this.label5.Location = new System.Drawing.Point(417, 298);

            this.label5.Name = "label5";

            this.label5.Size = new System.Drawing.Size(109, 13);

            this.label5.TabIndex = 94;

            this.label5.Text = "Blast Mask Cooldown";

            //
            // label6

            // 

            this.label6.AutoSize = true;

            this.label6.Location = new System.Drawing.Point(414, 320);

            this.label6.Name = "label6";

            this.label6.Size = new System.Drawing.Size(44, 13);

            this.label6.TabIndex = 95;

            this.label6.Text = "Frames:";

            //
            // BlastMaskFrames_Num

            // 

            this.BlastMaskFrames_Num.Location = new System.Drawing.Point(459, 318);

            this.BlastMaskFrames_Num.Maximum = new decimal(new int[] {

            65535,

            0,

            0,

            0});

            this.BlastMaskFrames_Num.Name = "BlastMaskFrames_Num";

            this.BlastMaskFrames_Num.Size = new System.Drawing.Size(67, 20);

            this.BlastMaskFrames_Num.TabIndex = 96;

            this.BlastMaskFrames_Num.ValueChanged += new System.EventHandler(this.numericUpDown1_ValueChanged);

            //
            // BlastMaskSeconds_Label

            // 

            this.BlastMaskSeconds_Label.AutoSize = true;

            this.BlastMaskSeconds_Label.Location = new System.Drawing.Point(456, 341);

            this.BlastMaskSeconds_Label.Name = "BlastMaskSeconds_Label";

            this.BlastMaskSeconds_Label.Size = new System.Drawing.Size(34, 13);

            this.BlastMaskSeconds_Label.TabIndex = 97;

            this.BlastMaskSeconds_Label.Text = "10:01";

            //
            // Targeting_Switch

            // 

            this.Targeting_Switch.AutoSize = true;

            this.Targeting_Switch.Location = new System.Drawing.Point(405, 52);

            this.Targeting_Switch.Name = "Targeting_Switch";

            this.Targeting_Switch.Size = new System.Drawing.Size(105, 17);

            this.Targeting_Switch.TabIndex = 98;

            this.Targeting_Switch.TabStop = true;

            this.Targeting_Switch.Tag = "Switch";

            this.Targeting_Switch.Text = "Switch Targeting";

            this.Targeting_Switch.UseVisualStyleBackColor = true;

            this.Targeting_Switch.CheckedChanged += new System.EventHandler(this.Targeting_Switch_CheckedChanged);

            //
            // Targeting_Hold

            // 

            this.Targeting_Hold.AutoSize = true;

            this.Targeting_Hold.Location = new System.Drawing.Point(405, 70);

            this.Targeting_Hold.Name = "Targeting_Hold";

            this.Targeting_Hold.Size = new System.Drawing.Size(95, 17);

            this.Targeting_Hold.TabIndex = 99;

            this.Targeting_Hold.TabStop = true;

            this.Targeting_Hold.Tag = "Hold";

            this.Targeting_Hold.Text = "Hold Targeting";

            this.Targeting_Hold.UseVisualStyleBackColor = true;

            this.Targeting_Hold.CheckedChanged += new System.EventHandler(this.Targeting_Switch_CheckedChanged);

            //
            // Main_Window
            //
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Main_Window";
            this.Text = "Majora\'s Mask Randomizer v1.0";
            this.Load += new System.EventHandler(this.Main_Window_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.BlastMaskFrames_Num)).EndInit();
            this.InitializePlandoControls();
            this.SetupMainWindowLayout();
            this.ResumeLayout(false);

                }

                #endregion

                private System.Windows.Forms.Label Pools_Label;
                private Majora_s_Mask_Randomizer_GUI.ThemedTabControl Pool_Tabs;
                private System.Windows.Forms.TextBox Create_Pool_Textbox;
                private System.Windows.Forms.Label Create_Pool_Label;
                private System.Windows.Forms.Button Create_Pool_Button;
                private System.Windows.Forms.ComboBox Remove_Pool_Combobox;
                private System.Windows.Forms.Button Remove_Pool_Button;
                private System.Windows.Forms.Label Remove_Pool_Label;
                private System.Windows.Forms.Label Change_Pool_Name_Label;
                private System.Windows.Forms.ComboBox Change_Pool_Name_Combobox;
                private System.Windows.Forms.TextBox Change_Pool_Name_Textbox;
                private System.Windows.Forms.Button Change_Pool_Name_Button;
                private System.Windows.Forms.Button Randomize_Button;
                private System.Windows.Forms.Button Save_Preset_Button;
                private System.Windows.Forms.Button Load_Preset_Button;
                private System.Windows.Forms.Button Open_Base_Rom_Button;
                private System.Windows.Forms.Label Base_Rom_Label;
                private System.Windows.Forms.OpenFileDialog Open_Base_Rom_Dialog;
                private System.Windows.Forms.SaveFileDialog Save_Presets_Dialog;
                private System.Windows.Forms.ComboBox Presets_Combobox;
                private System.Windows.Forms.Label Presets_Label;
                private System.Windows.Forms.TextBox Seed_Textbox;
                private System.Windows.Forms.Label Seed_Label;
                private System.Windows.Forms.Label label20;
                private System.Windows.Forms.ComboBox Logic_Combobox;
                private System.Windows.Forms.Label Logic_Label;
                private System.Windows.Forms.ToolTip Scrub_Sells_Beans_Tooltip;
                private System.Windows.Forms.Label Tunic_Label;
                private System.Windows.Forms.ColorDialog Tunic_ColorDialog;
                private System.Windows.Forms.Button Tunic_Button;
                private Majora_s_Mask_Randomizer_GUI.ThemedTabControl Items_Tab;
                private System.Windows.Forms.MenuStrip menuStrip1;
                private System.Windows.Forms.ToolStripMenuItem settingsToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem createWadToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem swampScrubSalesBeansToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem playAsKafeiToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem removeCutscenesToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem gCHudToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem respawnHPsToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem edibleMirrorShieldToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem keepRazorSwordOnSoTToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem oceanSpiderHouseAnyDayToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem respawnHCsToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem removeScrubSalesmanAfterTradingToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem pauseMenuColorsToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem walletToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem logicToolStripMenuItem;
                private System.Windows.Forms.ToolStripMenuItem cutscenesToolStripMenuItem;
                private System.Windows.Forms.Label label5;
                private System.Windows.Forms.Label label6;
                private System.Windows.Forms.NumericUpDown BlastMaskFrames_Num;
                private System.Windows.Forms.Label BlastMaskSeconds_Label;
                private System.Windows.Forms.RadioButton Targeting_Switch;
                private System.Windows.Forms.RadioButton Targeting_Hold;

        private const int SettingsLabelWidth = 132;
        private const int SettingsFieldWidth = 220;
        private const int SettingsButtonWidth = 80;
        private const int SettingsRenameFieldWidth = 120;
        private const int SettingsTargetingWidth = 150;
        private const int SettingsPresetsWidth = 260;
        private const int SettingsMiddleGroupHeight = 108;

        private void InitializePlandoControls()
        {
            Plando_Location_Combobox = new ComboBox
            {
                DropDownStyle = ComboBoxStyle.DropDownList
            };
            Plando_Item_Combobox = new ComboBox
            {
                DropDownStyle = ComboBoxStyle.DropDownList
            };
            Plando_Add_Button = new Button
            {
                Text = "Add"
            };
            Plando_Remove_Button = new Button
            {
                Text = "Remove"
            };
            Plando_List = new ListView
            {
                View = View.Details,
                FullRowSelect = true,
                MultiSelect = false,
                HideSelection = false,
                Height = 140
            };

            Plando_List.Columns.Add("Location", 220);
            Plando_List.Columns.Add("Item", 220);
            PoolListView.ConfigureDetailsList(Plando_List, showHeaders: true);
            Plando_Add_Button.Click += Plando_Add_Button_Click;
            Plando_Remove_Button.Click += Plando_Remove_Button_Click;
            Plando_List.SelectedIndexChanged += Plando_List_SelectedIndexChanged;
            Plando_List.DoubleClick += Plando_List_DoubleClick;
            Plando_List.Resize += (sender, args) => Resize_Plando_List();
        }

        private void SetupMainWindowLayout()
        {
            SuspendLayout();

            MinimumSize = new Size(1100, 640);
            ClientSize = new Size(1240, 760);

            _mainSplit = new SplitContainer
            {
                Dock = DockStyle.Fill,
                Orientation = Orientation.Vertical,
                SplitterWidth = 6
            };

            Items_Tab.Dock = DockStyle.Fill;

            Panel leftPanel = new Panel { Dock = DockStyle.Fill };
            leftPanel.Controls.Add(Items_Tab);
            _itemsTabHost = leftPanel;

            TableLayoutPanel rightPanel = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                ColumnCount = 1,
                RowCount = 2,
                Padding = new Padding(4, 0, 4, 4)
            };
            rightPanel.RowStyles.Add(new RowStyle(SizeType.Absolute, 52F));
            rightPanel.RowStyles.Add(new RowStyle(SizeType.Percent, 100F));

            Randomize_Button.Dock = DockStyle.Fill;
            Randomize_Button.Margin = new Padding(0, 4, 0, 4);
            Randomize_Button.Height = 44;
            UiTheme.StylePrimaryButton(Randomize_Button);

            GroupBox poolsGroup = new GroupBox
            {
                Text = "Item Pools",
                Dock = DockStyle.Fill,
                Padding = new Padding(8, 4, 8, 8)
            };
            Pool_Tabs.Dock = DockStyle.Fill;
            poolsGroup.Controls.Add(Pool_Tabs);

            rightPanel.Controls.Add(Randomize_Button, 0, 0);
            rightPanel.Controls.Add(poolsGroup, 0, 1);

            _mainSplit.Panel1.Controls.Add(leftPanel);
            _mainSplit.Panel2.Controls.Add(rightPanel);

            MainMenuStrip = menuStrip1;
            Controls.Add(_mainSplit);
            Controls.Add(menuStrip1);
            _mainSplit.Dock = DockStyle.Fill;
            menuStrip1.Dock = DockStyle.Top;
            settingsToolStripMenuItem.Visible = false;

            TabCategoryIcons.ConfigureCategoryTabs(Items_Tab);
            TabCategoryIcons.ConfigureTextTabs(Pool_Tabs);
            UiTheme.EnableDoubleBuffer(Items_Tab);
            AttachThemeToggleToTabStrip();
            Items_Tab.SelectedIndex = 0;

            ResumeLayout(true);

            UiTheme.ScheduleSplitLayout(this, _mainSplit, 920, 680, 280);
        }

        private void AddSettingsTab()
        {
            foreach (TabPage page in Items_Tab.TabPages)
            {
                if (page.Text == "Settings")
                {
                    return;
                }
            }

            TabPage settingsPage = new TabPage("Settings");
            Panel settingsScroll = new Panel
            {
                Dock = DockStyle.Fill,
                AutoScroll = true,
                Padding = new Padding(16, 12, 16, 12)
            };

            TableLayoutPanel content = BuildSettingsPanel();
            content.Dock = DockStyle.Top;
            content.AutoSize = true;
            content.AutoSizeMode = AutoSizeMode.GrowAndShrink;
            settingsScroll.Controls.Add(content);

            settingsPage.Controls.Add(settingsScroll);
            Items_Tab.TabPages.Insert(0, settingsPage);
        }

        private TableLayoutPanel BuildSettingsPanel()
        {
            TableLayoutPanel root = new TableLayoutPanel
            {
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                ColumnCount = 1,
                RowCount = 6,
                Padding = Padding.Empty
            };
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            root.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            root.Controls.Add(BuildPoolManagementGroup(), 0, 0);
            root.Controls.Add(BuildTargetingPresetsRow(), 0, 1);
            root.Controls.Add(BuildPlandoGroup(), 0, 2);
            root.Controls.Add(BuildRandomizerOptionsGroup(), 0, 3);
            root.Controls.Add(BuildPatchOptionsGroup(), 0, 4);
            root.Controls.Add(BuildBingoGroup(), 0, 5);

            return root;
        }

        private GroupBox BuildBingoGroup()
        {
            GroupBox group = CreateConfigGroupBox("Bingo Card");

            FlowLayoutPanel flow = new FlowLayoutPanel
            {
                Dock = DockStyle.Top,
                AutoSize = true,
                FlowDirection = FlowDirection.LeftToRight,
                WrapContents = true,
                Padding = new Padding(0, 2, 0, 0)
            };

            Label hint = new Label
            {
                AutoSize = true,
                Text = "Generate an SRL-style bingo card from enabled item pools.",
                Margin = new Padding(0, 6, 12, 0)
            };

            Button generateButton = new Button
            {
                Text = "Generate Bingo Card",
                AutoSize = true
            };
            StyleSettingsAction(generateButton);
            generateButton.Click += (sender, args) => OpenBingoCardDialog();

            flow.Controls.Add(hint);
            flow.Controls.Add(generateButton);
            group.Controls.Add(flow);
            return group;
        }

        private GroupBox BuildPoolManagementGroup()
        {
            GroupBox group = CreateConfigGroupBox("Pool Management");

            TableLayoutPanel table = new TableLayoutPanel
            {
                Dock = DockStyle.Top,
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                ColumnCount = 3,
                RowCount = 3,
                Padding = new Padding(0, 2, 0, 0)
            };
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsLabelWidth));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsFieldWidth));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsButtonWidth));

            for (int i = 0; i < 3; i++)
            {
                table.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            }

            AddSettingsFieldRow(table, 0, "Create pool", Create_Pool_Textbox, Create_Pool_Button);
            AddSettingsFieldRow(table, 1, "Remove pool", Remove_Pool_Combobox, Remove_Pool_Button);

            Label changeLabel = CreateFieldLabel("Rename pool");
            FlowLayoutPanel renameFields = new FlowLayoutPanel
            {
                AutoSize = true,
                FlowDirection = FlowDirection.LeftToRight,
                WrapContents = false,
                Margin = new Padding(0, 2, 0, 2)
            };
            Change_Pool_Name_Combobox.Width = SettingsRenameFieldWidth;
            Change_Pool_Name_Textbox.Width = SettingsRenameFieldWidth;
            Change_Pool_Name_Combobox.Margin = new Padding(0, 0, 6, 0);
            Change_Pool_Name_Textbox.Margin = new Padding(0, 0, 6, 0);
            PrepareReparentedControl(Change_Pool_Name_Combobox);
            PrepareReparentedControl(Change_Pool_Name_Textbox);
            PrepareReparentedControl(Change_Pool_Name_Button);
            StyleSettingsInput(Change_Pool_Name_Combobox);
            StyleSettingsInput(Change_Pool_Name_Textbox);
            Change_Pool_Name_Combobox.Width = SettingsRenameFieldWidth;
            Change_Pool_Name_Textbox.Width = SettingsRenameFieldWidth;
            StyleSettingsAction(Change_Pool_Name_Button);
            renameFields.Controls.Add(Change_Pool_Name_Combobox);
            renameFields.Controls.Add(Change_Pool_Name_Textbox);
            renameFields.Controls.Add(Change_Pool_Name_Button);
            table.Controls.Add(changeLabel, 0, 2);
            table.Controls.Add(renameFields, 1, 2);
            table.SetColumnSpan(renameFields, 2);

            group.Controls.Add(table);
            return group;
        }

        private GroupBox BuildPlandoGroup()
        {
            Plando_Group = CreateConfigGroupBox("Plando");

            TableLayoutPanel table = new TableLayoutPanel
            {
                Dock = DockStyle.Top,
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                ColumnCount = 3,
                RowCount = 7,
                Padding = new Padding(0, 2, 0, 0)
            };
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsLabelWidth));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsFieldWidth));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsButtonWidth));

            for (int i = 0; i < 7; i++)
            {
                table.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            }

            AddSettingsFieldRow(table, 0, "Location", Plando_Location_Combobox, Plando_Add_Button);
            AddSettingsFieldRow(table, 1, "Item", Plando_Item_Combobox, Plando_Remove_Button);

            int plandoListWidth = SettingsLabelWidth + SettingsFieldWidth + SettingsButtonWidth;
            PrepareReparentedControl(Plando_List);
            Plando_List.Width = plandoListWidth;
            Plando_List.Margin = new Padding(0, 6, 0, 0);
            table.Controls.Add(Plando_List, 0, 2);
            table.SetColumnSpan(Plando_List, 3);

            WarnPoolBalance_Checkbox = new CheckBox
            {
                Text = "Warn before randomize when pool issues remain",
                AutoSize = true,
                Margin = new Padding(0, 8, 0, 4)
            };
            table.Controls.Add(WarnPoolBalance_Checkbox, 0, 3);
            table.SetColumnSpan(WarnPoolBalance_Checkbox, 3);

            Label issuesLabel = CreateFieldLabel("Pool issues");
            issuesLabel.Margin = new Padding(0, 4, 8, 4);
            table.Controls.Add(issuesLabel, 0, 4);

            Pool_Issues_List = new ListView
            {
                Height = 96,
                Width = plandoListWidth
            };
            Pool_Issues_List.Columns.Add("Issue", plandoListWidth - 8);
            PoolListView.ConfigureIssueList(Pool_Issues_List);
            PrepareReparentedControl(Pool_Issues_List);
            Pool_Issues_List.Margin = new Padding(0, 0, 0, 4);
            table.Controls.Add(Pool_Issues_List, 0, 5);
            table.SetColumnSpan(Pool_Issues_List, 3);

            FlowLayoutPanel issueActions = new FlowLayoutPanel
            {
                AutoSize = true,
                FlowDirection = FlowDirection.LeftToRight,
                WrapContents = false,
                Margin = new Padding(0, 0, 0, 0)
            };
            Pool_Issue_Fix_Button = new Button { Text = "Resolve" };
            Pool_Issue_Ignore_Button = new Button { Text = "Ignore" };
            StyleSettingsAction(Pool_Issue_Fix_Button);
            StyleSettingsAction(Pool_Issue_Ignore_Button);
            Pool_Issue_Fix_Button.Margin = new Padding(0, 0, 8, 0);
            issueActions.Controls.Add(Pool_Issue_Fix_Button);
            issueActions.Controls.Add(Pool_Issue_Ignore_Button);
            table.Controls.Add(issueActions, 0, 6);
            table.SetColumnSpan(issueActions, 3);

            Plando_Group.Controls.Add(table);
            return Plando_Group;
        }

        private GroupBox BuildPatchOptionsGroup()
        {
            GroupBox group = CreateConfigGroupBox("Patch Options");

            TableLayoutPanel table = new TableLayoutPanel
            {
                Dock = DockStyle.Top,
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                ColumnCount = 2,
                RowCount = 5,
                Padding = new Padding(0, 2, 0, 0)
            };
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 245F));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 245F));

            for (int i = 0; i < 5; i++)
            {
                table.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            }

            AddPatchOption(table, 0, 0, "Create WAD", createWadToolStripMenuItem);
            AddPatchOption(table, 1, 0, "Swamp Scrub Sales Beans", swampScrubSalesBeansToolStripMenuItem);
            AddPatchOption(table, 0, 1, "Play as Kafei", playAsKafeiToolStripMenuItem);
            AddPatchOption(table, 1, 1, "GC HUD", gCHudToolStripMenuItem);
            AddPatchOption(table, 0, 2, "Respawn HPs", respawnHPsToolStripMenuItem);
            AddPatchOption(table, 1, 2, "Edible Mirror Shield", edibleMirrorShieldToolStripMenuItem);
            AddPatchOption(table, 0, 3, "Keep Razor Sword on SoT", keepRazorSwordOnSoTToolStripMenuItem);
            AddPatchOption(table, 1, 3, "Ocean Spider House Any Day", oceanSpiderHouseAnyDayToolStripMenuItem);
            AddPatchOption(table, 0, 4, "Respawn HCs", respawnHCsToolStripMenuItem);
            AddPatchOption(table, 1, 4, "Remove Scrub Salesmen after trading", removeScrubSalesmanAfterTradingToolStripMenuItem);

            group.Controls.Add(table);
            return group;
        }

        private CheckBox AddPatchOption(TableLayoutPanel table, int column, int row, string text, ToolStripMenuItem menuItem)
        {
            CheckBox checkBox = new CheckBox
            {
                Text = text,
                AutoSize = true,
                Checked = menuItem.Checked,
                Anchor = AnchorStyles.Left,
                Margin = new Padding(4, 4, 12, 4)
            };

            checkBox.CheckedChanged += (sender, args) =>
            {
                if (menuItem.Checked != checkBox.Checked)
                {
                    menuItem.Checked = checkBox.Checked;
                }
            };
            menuItem.CheckedChanged += (sender, args) =>
            {
                if (checkBox.Checked != menuItem.Checked)
                {
                    checkBox.Checked = menuItem.Checked;
                }
            };

            table.Controls.Add(checkBox, column, row);
            return checkBox;
        }

        private static void PrepareReparentedControl(Control control)
        {
            control.Dock = DockStyle.None;
            control.Anchor = AnchorStyles.Top | AnchorStyles.Left;
            control.Location = Point.Empty;
        }

        private static void AddSettingsFieldRow(
            TableLayoutPanel table,
            int row,
            string labelText,
            Control field,
            Control action)
        {
            table.Controls.Add(CreateFieldLabel(labelText), 0, row);
            PrepareReparentedControl(field);
            StyleSettingsInput(field);
            field.Margin = new Padding(0, 2, 8, 2);
            table.Controls.Add(field, 1, row);
            PrepareReparentedControl(action);
            StyleSettingsAction(action);
            table.Controls.Add(action, 2, row);
        }

        private static void StyleSettingsAction(Control action)
        {
            action.Margin = new Padding(0, 2, 0, 2);
            action.Anchor = AnchorStyles.Left | AnchorStyles.Top;
            if (action is Button button)
            {
                button.AutoSize = false;
                button.Width = SettingsButtonWidth;
            }
        }

        private static void StyleSettingsInput(Control control)
        {
            control.Anchor = AnchorStyles.Left | AnchorStyles.Top;
            if (control is TextBox || control is ComboBox)
            {
                control.Width = SettingsFieldWidth;
            }
        }

        private Control BuildTargetingPresetsRow()
        {
            TableLayoutPanel row = new TableLayoutPanel
            {
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                ColumnCount = 2,
                RowCount = 1,
                Margin = new Padding(0, 8, 0, 0),
                Padding = Padding.Empty,
                Dock = DockStyle.Top
            };
            row.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsTargetingWidth + 24));
            row.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsPresetsWidth));
            row.RowStyles.Add(new RowStyle(SizeType.Absolute, SettingsMiddleGroupHeight));

            GroupBox targeting = CreateConfigGroupBox("Targeting", dockTop: false);
            ConfigureFixedSettingsGroup(targeting, SettingsTargetingWidth, SettingsMiddleGroupHeight);
            targeting.Margin = new Padding(0, 0, 24, 8);
            PrepareReparentedControl(Targeting_Switch);
            PrepareReparentedControl(Targeting_Hold);
            Targeting_Switch.AutoSize = true;
            Targeting_Hold.AutoSize = true;
            Targeting_Switch.Location = new Point(10, 24);
            Targeting_Hold.Location = new Point(10, 52);
            targeting.Controls.Add(Targeting_Switch);
            targeting.Controls.Add(Targeting_Hold);

            GroupBox presets = CreateConfigGroupBox("Presets", dockTop: false);
            ConfigureFixedSettingsGroup(presets, SettingsPresetsWidth, SettingsMiddleGroupHeight);
            presets.Margin = new Padding(0, 0, 0, 8);
            PrepareReparentedControl(Presets_Combobox);
            PrepareReparentedControl(Load_Preset_Button);
            PrepareReparentedControl(Save_Preset_Button);
            Presets_Combobox.Width = SettingsFieldWidth;
            Presets_Combobox.Location = new Point(10, 22);
            StyleSettingsAction(Load_Preset_Button);
            StyleSettingsAction(Save_Preset_Button);
            Load_Preset_Button.Text = "Load";
            Save_Preset_Button.Text = "Save";
            Load_Preset_Button.Location = new Point(10, 52);
            Save_Preset_Button.Location = new Point(100, 52);
            presets.Controls.Add(Presets_Combobox);
            presets.Controls.Add(Load_Preset_Button);
            presets.Controls.Add(Save_Preset_Button);

            row.Controls.Add(targeting, 0, 0);
            row.Controls.Add(presets, 1, 0);
            return row;
        }

        private GroupBox BuildRandomizerOptionsGroup()
        {
            GroupBox group = CreateConfigGroupBox("Randomizer Options");

            TableLayoutPanel table = new TableLayoutPanel
            {
                Dock = DockStyle.Top,
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                ColumnCount = 2,
                RowCount = 5,
                Padding = new Padding(0, 2, 0, 0)
            };
            table.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, SettingsLabelWidth));
            table.ColumnStyles.Add(new ColumnStyle(SizeType.AutoSize));
            for (int i = 0; i < 5; i++)
            {
                table.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            }

            table.Controls.Add(CreateFieldLabel("Logic"), 0, 0);
            PrepareReparentedControl(Logic_Combobox);
            StyleSettingsInput(Logic_Combobox);
            Logic_Combobox.Margin = new Padding(0, 2, 0, 6);
            table.Controls.Add(Logic_Combobox, 1, 0);

            table.Controls.Add(CreateFieldLabel("Base ROM"), 0, 1);
            FlowLayoutPanel baseRomPanel = new FlowLayoutPanel
            {
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                FlowDirection = FlowDirection.TopDown,
                WrapContents = false,
                Margin = new Padding(0, 2, 0, 6)
            };
            PrepareReparentedControl(Open_Base_Rom_Button);
            PrepareReparentedControl(Base_Rom_Label);
            Open_Base_Rom_Button.AutoSize = true;
            Open_Base_Rom_Button.Anchor = AnchorStyles.Left | AnchorStyles.Top;
            Open_Base_Rom_Button.Margin = new Padding(0, 0, 0, 4);
            Base_Rom_Label.AutoSize = true;
            Base_Rom_Label.MaximumSize = new Size(SettingsFieldWidth, 0);
            Base_Rom_Label.ForeColor = UiTheme.Current.HintForeColor;
            Base_Rom_Label.Font = UiTheme.Current.HintFont;
            Base_Rom_Label.Margin = Padding.Empty;
            Base_Rom_Label.Anchor = AnchorStyles.Left | AnchorStyles.Top;
            baseRomPanel.Controls.Add(Open_Base_Rom_Button);
            baseRomPanel.Controls.Add(Base_Rom_Label);
            table.Controls.Add(baseRomPanel, 1, 1);

            table.Controls.Add(CreateFieldLabel("Blast mask cooldown"), 0, 2);
            FlowLayoutPanel blastPanel = new FlowLayoutPanel
            {
                AutoSize = true,
                FlowDirection = FlowDirection.LeftToRight,
                WrapContents = false,
                Margin = new Padding(0, 2, 0, 6)
            };
            PrepareReparentedControl(BlastMaskFrames_Num);
            PrepareReparentedControl(label6);
            PrepareReparentedControl(BlastMaskSeconds_Label);
            BlastMaskFrames_Num.Margin = new Padding(0, 0, 8, 0);
            label6.AutoSize = true;
            label6.Text = "frames";
            label6.Margin = new Padding(0, 4, 4, 0);
            BlastMaskSeconds_Label.AutoSize = true;
            BlastMaskSeconds_Label.Margin = new Padding(0, 4, 0, 0);
            blastPanel.Controls.Add(BlastMaskFrames_Num);
            blastPanel.Controls.Add(label6);
            blastPanel.Controls.Add(BlastMaskSeconds_Label);
            table.Controls.Add(blastPanel, 1, 2);

            table.Controls.Add(CreateFieldLabel("Seed"), 0, 3);
            PrepareReparentedControl(Seed_Textbox);
            StyleSettingsInput(Seed_Textbox);
            Seed_Textbox.Margin = new Padding(0, 2, 0, 2);
            table.Controls.Add(Seed_Textbox, 1, 3);

            PrepareReparentedControl(label20);
            label20.AutoSize = true;
            label20.Font = UiTheme.Current.HintFont;
            label20.ForeColor = UiTheme.Current.HintForeColor;
            label20.MaximumSize = new Size(SettingsFieldWidth, 0);
            label20.Margin = new Padding(0, 0, 0, 2);
            table.Controls.Add(label20, 1, 4);

            group.Controls.Add(table);
            return group;
        }

        private static Label CreateFieldLabel(string text)
        {
            return new Label
            {
                Text = text,
                AutoSize = true,
                Anchor = AnchorStyles.Left,
                TextAlign = ContentAlignment.MiddleLeft,
                Font = UiTheme.Current.BaseFont,
                Margin = new Padding(0, 6, 8, 6)
            };
        }

        private static void ConfigureFixedSettingsGroup(GroupBox group, int width, int height)
        {
            group.AutoSize = false;
            group.AutoSizeMode = AutoSizeMode.GrowOnly;
            group.Dock = DockStyle.None;
            group.Size = new Size(width, height);
            group.Padding = Padding.Empty;
        }

        private static GroupBox CreateConfigGroupBox(string title, bool dockTop = true)
        {
            return new GroupBox
            {
                Text = title,
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                Dock = dockTop ? DockStyle.Top : DockStyle.None,
                Padding = new Padding(10, 6, 12, 10),
                Margin = new Padding(0, 0, 0, 8)
            };
        }
    }
}
