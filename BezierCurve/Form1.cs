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
            Draw();
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
            Cursor.Current = Cursors.Hand;
            if (e.Button == MouseButtons.Right && EditMode)
            {
                int ind;
                int.TryParse(l.Name.Split('_')[1], out ind);
                Controls.Remove(l);
                Points[ind] = new Point(-1, -1);
            }
        }

        private void Draw()
        {
            Graphics graphics = Graphics.FromImage(pictureBox1.Image);
            graphics.Clear(Color.AliceBlue);
            BezierCurve(Points);
        }

        private Point Middle(Point p1, Point p2)
        {
            int x, y;
            if (p1.X < p2.X)
                x = (p2.X - p1.X) / 2 + p1.X;
            else
                x = (p1.X - p2.X) / 2 + p2.X;
            if (p1.Y < p2.Y)
                y = (p2.Y - p1.Y) / 2 + p1.Y;
            else
                y = (p1.Y - p2.Y) / 2 + p2.Y;
            return new Point(x, y);
        }

        private void BezierCurve(List<Point> points)
        {
            List<Point> curr_points = new List<Point>(Points);

            curr_points.RemoveAll(item => item.X == -1);

            Bitmap bmp = pictureBox1.Image as Bitmap;

            var temp_points = new List<Point>(curr_points);

            if (curr_points.Count > 4)
            {
                int count = 0;
                if (curr_points.Count < 8)
                {
                    count = 8 - curr_points.Count;
                }
                else
                {
                    if (curr_points.Count % 2 == 1)
                        count = 1;
                    else
                        count = 0;
                }

                for (int i = 1; i <= count; i++)
                    temp_points.Add(curr_points[curr_points.Count - 1]);

                for (int i = 2; i < temp_points.Count - 3; i += 2)
                {
                    var p = Middle(temp_points[i], temp_points[i + 1]);
                    temp_points.Insert(i + 1, p);
                    i += 1;
                }
            }

            int ind = 0;
            Point p0, p1, p2, p3;
            while (temp_points.Count > ind + 3)
            {
                p0 = temp_points[ind];
                p1 = temp_points[ind + 1];
                p2 = temp_points[ind + 2];
                p3 = temp_points[ind + 3];
                ind += 3;
                for (double t = 0; t <= 1; t += 0.001)
                {
                    double curr_x = (1 - t) * (1 - t) * (1 - t) * p0.X +
                                3 * t * (1 - t) * (1 - t) * p1.X +
                                3 * t * t * (1 - t) * p2.X +
                                t * t * t * p3.X;

                    double curr_y = (1 - t) * (1 - t) * (1 - t) * p0.Y +
                                3 * t * (1 - t) * (1 - t) * p1.Y +
                                3 * t * t * (1 - t) * p2.Y +
                                t * t * t * p3.Y;

                    bmp.SetPixel((int)curr_x, (int)curr_y, Color.Red);
                }
            }
            pictureBox1.Refresh();
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
                Draw();
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

                if (lst.Count >= 4)
                {
                    EditMode = false;
                    editToolStripMenuItem.Text = "Edit";
                    Draw();
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

        private void clearToolStripMenuItem_Click_1(object sender, EventArgs e)
        {
            Points.Clear();
            editToolStripMenuItem.Text = "Edit";
            EditMode = false;
            List<Label> l = new List<Label>();
            foreach (Label obj in Controls.OfType<Label>())
            {
                int ind;
                int.TryParse(obj.Name.Split('_')[1], out ind);
                if (ind >= 4)
                {
                    l.Add(obj);
                    //Controls.Remove(obj);

                }

            }
            for (int i = 0; i < l.Count; i++)
                Controls.Remove(l[i]);
            lcount = 4;
            l_0.Location = new Point(63, 274);
            l_1.Location = new Point(142, 150);
            l_2.Location = new Point(295, 302);
            l_3.Location = new Point(366, 196);
            Points.Add(l_0.Location);
            Points.Add(l_1.Location);
            Points.Add(l_2.Location);
            Points.Add(l_3.Location);
            pictureBox1.Image = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            Graphics graphics = Graphics.FromImage(pictureBox1.Image);
            graphics.Clear(Color.AliceBlue);
            Draw();
        }
    }
}
