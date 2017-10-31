using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BezierCurve
{
    public partial class Form1 : Form
    {

        private Point StartPoint;
        private bool IsMouseDown = false;
        private List<Point> Points = new List<Point>();
        private List<Point> Curve = new List<Point>();
        private bool EditMode = false;
        private int lcount = 4;

        public Form1()
        {
            InitializeComponent();
            Points.Add(l_0.Location);
            Points.Add(l_1.Location);
            Points.Add(l_2.Location);
            Points.Add(l_3.Location);
            pictureBox1.Image = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            Graphics graphics = Graphics.FromImage(pictureBox1.Image);
            graphics.Clear(Color.AliceBlue);
            bezier();
            draw();
        }

        private void l_MouseDown(object sender, MouseEventArgs e)
        {
            Label l = sender as Label;
            if (e.Button == MouseButtons.Left)
            {
                Cursor.Current = Cursors.Hand;
                IsMouseDown = true;
                l.BringToFront();
                StartPoint = e.Location;
            }
            if (e.Button == MouseButtons.Right && EditMode)
            {
                int ind;
                int.TryParse(l.Name.Split('_')[1], out ind);
                Controls.Remove(l);
                Points[ind] = new Point(-1, -1);
            }
        }

        private void l_MouseMove(object sender, MouseEventArgs e)
        {
            Label l = sender as Label;
            if (IsMouseDown && !EditMode)
            {
                l.Left = e.X + l.Left - StartPoint.X;
                l.Top = e.Y + l.Top - StartPoint.Y;
                int ind;
                int.TryParse(l.Name.Split('_')[1], out ind);
                Points[ind] = new Point(l.Location.X, l.Location.Y);
                bezier();
                draw();
            }
            else if (IsMouseDown && EditMode)
            {
                l.Left = e.X + l.Left - StartPoint.X;
                l.Top = e.Y + l.Top - StartPoint.Y;
                int ind;
                int.TryParse(l.Name.Split('_')[1], out ind);
                Points[ind] = new Point(l.Location.X, l.Location.Y);
            }
        }

        private void l_MouseUp(object sender, MouseEventArgs e)
        {
            IsMouseDown = false;
        }

        private void draw()
        {
            Graphics graphics = Graphics.FromImage(pictureBox1.Image);
            graphics.Clear(Color.AliceBlue);
            Pen pen = new Pen(Color.Black);
            pen.StartCap = pen.EndCap = LineCap.Round;
            for (int i = 1; i < Curve.Count; i++) 
            {
                graphics.DrawEllipse(pen, new Rectangle(Curve[i].X, Curve[i].Y, 1, 1));
                graphics.DrawLine(pen, Curve[i], Curve[i - 1]);
            }

            pictureBox1.Refresh();
        }

        private void bezier()
        {
            Curve.Clear();
            List<Point> lst = new List<Point>(Points);
            lst.RemoveAll(item => item.X == -1);
            int currX;
            int currY;
            for (int i = 3; i < lst.Count; i += 3)
            {
                for (double t = 0; t <= 1; t += 0.0001)
                {

                    /*x = (1 − t)³⋅x1 + 3⋅(1 − t)²⋅t⋅x2 + 3⋅(1 − t)⋅t²⋅x3 + t³⋅x4,
                   y = (1 − t)³⋅y1 + 3⋅(1 − t)²⋅t⋅y2 + 3⋅(1 − t)⋅t²⋅y3 + t³⋅y4,*/


                    currX = (int)((1 - t) * (1 - t) * (1 - t) * lst[i - 3].X +
                       3 * t * (1 - t) * (1 - t) * lst[i - 2].X +
                       3 * t * t * (1 - t) * lst[i - 1].X +
                       t * t * t * lst[i].X);
                    currY = (int)((1 - t) * (1 - t) * (1 - t) * lst[i - 3].Y +
                        3 * t * (1 - t) * (1 - t) * lst[i - 2].Y +
                        3 * t * t * (1 - t) * lst[i - 1].Y +
                        t * t * t * lst[i].Y);
                    Curve.Add(new Point(currX, currY));
                }
            }
        }

        private void clearToolStripMenuItem_Click(object sender, EventArgs e)
        {
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void editToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (!EditMode)
            {
                EditMode = true;
                editToolStripMenuItem.Text = "Display mode";
                Graphics graphics = Graphics.FromImage(pictureBox1.Image);
                graphics.Clear(Color.AliceBlue);
                pictureBox1.Refresh();
            }
            else
            {
                List<Point> lst = new List<Point>(Points);
                lst.RemoveAll(item => item.X == -1);

                if ((lst.Count - 1) % 3 == 0 && lst.Count >= 4)
                {
                    EditMode = false;
                    editToolStripMenuItem.Text = "Edit";
                    bezier();
                    draw();
                }
            }
        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left && EditMode)
            {
                Label l = new Label();
                l.Name = "l_" + lcount.ToString();
                l.Location = new Point(e.X, e.Y);
                Points.Add(l.Location);

                lcount += 1;
                Random r = new Random();
                l.BackColor = Color.FromArgb(r.Next(200), r.Next(200), r.Next(200));
                l.AutoSize = false;
                l.Size = new Size(10, 10);
                l.ForeColor = Color.Transparent;
                l.MouseDown += new MouseEventHandler(this.l_MouseDown);
                l.MouseMove += new MouseEventHandler(this.l_MouseMove);
                l.MouseUp += new MouseEventHandler(this.l_MouseUp);
                Controls.Add(l);
                l.BringToFront();
            }
        }
    }
}
