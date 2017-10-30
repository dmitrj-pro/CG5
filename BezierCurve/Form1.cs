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
        public Form1()
        {
            InitializeComponent();
            LabelsToList();
            pictureBox1.Image = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            Graphics graphics = Graphics.FromImage(pictureBox1.Image);
            graphics.Clear(Color.AliceBlue);
            bezier();
            draw();
        }

        private void LabelsToList()
        {
            Points.Clear();
            Points.Add(l_0.Location);
            Points.Add(l_1.Location);
            Points.Add(l_2.Location);
            Points.Add(l_3.Location);
        }

        private void l_MouseDown(object sender, MouseEventArgs e)
        {
            Label l = sender as Label;
            if (e.Button == MouseButtons.Left)
            {
                IsMouseDown = true;
                l.BringToFront();
                StartPoint = e.Location;
            }
            Cursor.Current = Cursors.Hand;
        }

        private void l_MouseMove(object sender, MouseEventArgs e)
        {
            Label l = sender as Label;
            if (IsMouseDown)
            {
                l.Left = e.X + l.Left - StartPoint.X;
                l.Top = e.Y + l.Top - StartPoint.Y;
                LabelsToList();
                bezier();
                draw();

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
            int currX;
            int currY;
            int ind = 0;
            for (double t = 0; t <= 1; t += 0.001)
            {

                /*x = (1 − t)³⋅x1 + 3⋅(1 − t)²⋅t⋅x2 + 3⋅(1 − t)⋅t²⋅x3 + t³⋅x4,
                   y = (1 − t)³⋅y1 + 3⋅(1 − t)²⋅t⋅y2 + 3⋅(1 − t)⋅t²⋅y3 + t³⋅y4,*/

                currX = (int)((1 - t) * (1 - t) * (1 - t) * Points[ind].X +
                        3 * t * (1 - t) * (1 - t) * Points[ind + 1].X +
                        3 * t * t * (1 - t) * Points[ind + 2].X +
                        t * t * t * Points[ind + 3].X);
                currY = (int)((1 - t) * (1 - t) * (1 - t) * Points[ind].Y +
                    3 * t * (1 - t) * (1 - t) * Points[ind + 1].Y +
                    3 * t * t * (1 - t) * Points[ind + 2].Y +
                    t * t * t * Points[ind + 3].Y);
                Curve.Add(new Point(currX, currY));
            }
            
        }

        private void clearToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
