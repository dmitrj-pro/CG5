namespace BezierCurve
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.menuToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.clearToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.l_1 = new System.Windows.Forms.Label();
            this.l_0 = new System.Windows.Forms.Label();
            this.l_3 = new System.Windows.Forms.Label();
            this.l_2 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.SystemColors.Control;
            this.pictureBox1.Location = new System.Drawing.Point(12, 27);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(460, 422);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(484, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // menuToolStripMenuItem
            // 
            this.menuToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.clearToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.menuToolStripMenuItem.Name = "menuToolStripMenuItem";
            this.menuToolStripMenuItem.Size = new System.Drawing.Size(50, 20);
            this.menuToolStripMenuItem.Text = "Menu";
            // 
            // clearToolStripMenuItem
            // 
            this.clearToolStripMenuItem.Name = "clearToolStripMenuItem";
            this.clearToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.clearToolStripMenuItem.Text = "Clear";
            this.clearToolStripMenuItem.Click += new System.EventHandler(this.clearToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // l_1
            // 
            this.l_1.BackColor = System.Drawing.Color.Blue;
            this.l_1.Location = new System.Drawing.Point(142, 150);
            this.l_1.Name = "l_1";
            this.l_1.Size = new System.Drawing.Size(10, 10);
            this.l_1.TabIndex = 2;
            this.l_1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.l_MouseDown);
            this.l_1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.l_MouseMove);
            this.l_1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.l_MouseUp);
            // 
            // l_0
            // 
            this.l_0.BackColor = System.Drawing.Color.Green;
            this.l_0.ForeColor = System.Drawing.Color.Transparent;
            this.l_0.Location = new System.Drawing.Point(63, 274);
            this.l_0.Name = "l_0";
            this.l_0.Size = new System.Drawing.Size(10, 10);
            this.l_0.TabIndex = 3;
            this.l_0.MouseDown += new System.Windows.Forms.MouseEventHandler(this.l_MouseDown);
            this.l_0.MouseMove += new System.Windows.Forms.MouseEventHandler(this.l_MouseMove);
            this.l_0.MouseUp += new System.Windows.Forms.MouseEventHandler(this.l_MouseUp);
            // 
            // l_3
            // 
            this.l_3.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(0)))), ((int)(((byte)(192)))));
            this.l_3.Location = new System.Drawing.Point(366, 196);
            this.l_3.Name = "l_3";
            this.l_3.Size = new System.Drawing.Size(10, 10);
            this.l_3.TabIndex = 4;
            this.l_3.MouseDown += new System.Windows.Forms.MouseEventHandler(this.l_MouseDown);
            this.l_3.MouseMove += new System.Windows.Forms.MouseEventHandler(this.l_MouseMove);
            this.l_3.MouseUp += new System.Windows.Forms.MouseEventHandler(this.l_MouseUp);
            // 
            // l_2
            // 
            this.l_2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(128)))));
            this.l_2.Location = new System.Drawing.Point(295, 302);
            this.l_2.Name = "l_2";
            this.l_2.Size = new System.Drawing.Size(10, 10);
            this.l_2.TabIndex = 5;
            this.l_2.MouseDown += new System.Windows.Forms.MouseEventHandler(this.l_MouseDown);
            this.l_2.MouseMove += new System.Windows.Forms.MouseEventHandler(this.l_MouseMove);
            this.l_2.MouseUp += new System.Windows.Forms.MouseEventHandler(this.l_MouseUp);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(484, 461);
            this.Controls.Add(this.l_2);
            this.Controls.Add(this.l_3);
            this.Controls.Add(this.l_0);
            this.Controls.Add(this.l_1);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.menuStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MainMenuStrip = this.menuStrip1;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem menuToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem clearToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.Label l_1;
        private System.Windows.Forms.Label l_0;
        private System.Windows.Forms.Label l_3;
        private System.Windows.Forms.Label l_2;
    }
}

