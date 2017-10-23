using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lab4
{
    public partial class Form1 : Form
    {
        private int show_mess = 1;
        private int hint = 0;
        private int queen_count = 0;
        private Myboard myboard = new Myboard();
        public Form1()
        {
            InitializeComponent();
            this.Text = "Eight Queens by Zhuohao Yang";
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            Graphics g = e.Graphics;
            Pen pen = new Pen(Brushes.Black, 0.01f);
            Font myfont = new Font("Arial", 30, FontStyle.Bold);
            SolidBrush blockbrush = new SolidBrush(Color.White);
            SolidBrush queenbrush = new SolidBrush(Color.Black);
            string mystr = "You have ";
            mystr = mystr + queen_count + " queens on the board";
            g.DrawString(mystr, Font, queenbrush, 210, 35);
            //if (hint == 1)
            //    g.DrawString("hello", Font, queenbrush, 500, 500);

            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if ((i % 2 == 0 && j % 2 == 0) || (i % 2 == 1 && j % 2 == 1))
                    {
                        if (hint == 1 && myboard.board[i][j].capture > 0)
                            blockbrush.Color = Color.Red;
                        else
                            blockbrush.Color = Color.White;
                        queenbrush.Color = Color.Black;
                    }
                    else
                    {
                        if (hint == 1 && myboard.board[i][j].capture > 0)
                        {
                            blockbrush.Color = Color.Red;
                            queenbrush.Color = Color.Black;
                        }
                        else
                        {
                            blockbrush.Color = Color.Black;
                            queenbrush.Color = Color.White;
                        }
                    }
                    g.FillRectangle(blockbrush, 100 + 50 * i, 100 + 50 * j, 50, 50);
                    g.DrawRectangle(pen, 100 + 50 * i, 100 + 50 * j, 50, 50);
                    if (myboard.board[i][j].queen == 1)
                        g.DrawString("Q", myfont, queenbrush, 100 + 50 * i, 100 + 50 * j);
                }
            }
        }
        private void Form1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void Form1_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                for (int i = 0; i < 8; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        if (e.X >= 100 + 50 * i && e.X < 150 + 50 * i && e.Y >= 100 + 50 * j && e.Y < 150 + 50 * j)
                        {
                            //System.Media.SystemSounds.Beep.Play();
                            if (myboard.board[i][j].queen == 0 && myboard.board[i][j].capture == 0)
                            {
                                myboard.board[i][j].queen = 1;
                                queen_count++;
                                myboard.captureloc(i, j);
                                show_mess = 1;
                                this.Invalidate();
                            }
                            else
                                System.Media.SystemSounds.Beep.Play();
                        }
                    }
                }
                if (queen_count == 8 && show_mess == 1)
                {
                    MessageBox.Show("You did it!");
                    show_mess = 0;
                }
            }
            else if (e.Button == MouseButtons.Right)
            {
                for (int i = 0; i < 8; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        if (e.X >= 100 + 50 * i && e.X < 150 + 50 * i && e.Y >= 100 + 50 * j && e.Y < 150 + 50 * j)
                        {
                            if (myboard.board[i][j].queen == 1)
                            { 
                                myboard.board[i][j].queen = 0;
                                queen_count--;
                                myboard.uncaptureloc(i, j);
                                this.Invalidate();
                            }
                        }
                    }
                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    myboard.board[i][j].queen = 0;
                    myboard.board[i][j].capture = 0;
                    queen_count = 0;
                }
            }
            this.Invalidate();
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBox1.Checked)
                hint = 1;
            else
                hint = 0;
            this.Invalidate();
        }
    }
    public class Myblock
    {
        public int queen;
        public int capture;
        public Myblock()
        {
            queen = 0;
            capture = 0;
        }
    }
    public class Myboard
    {
        public List<List<Myblock>> board= new List<List<Myblock>>();
        public void captureloc(int x, int y)
        {
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (i == x || j == y || x - i == y - j || i - x == y - j)
                        board[i][j].capture++;
                }
            }
        }
        public void uncaptureloc(int x, int y)
        {
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (i == x || j == y || x - i == y - j || i - x == y - j)
                        board[i][j].capture--;
                }
            }
        }
        public Myboard()
        {
            for (int j = 0; j < 8; j++)
            {
                List<Myblock> blockrow = new List<Myblock>();
                for (int i = 0; i < 8; i++)
                {
                    Myblock singleblock = new Myblock();
                    blockrow.Add(singleblock);
                }
                this.board.Add(blockrow);
            }
        }
    }
}
