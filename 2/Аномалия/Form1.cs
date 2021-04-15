using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Аномалия
{
    public partial class Form1 : Form
    {
        private int data1 = 192;
        private int data2 = 134;
        private int data3 = 251;
        string str1;
        string str2;
        string str3;
        int con1, con2, con3;

        private void button2_Click(object sender, EventArgs e)
        {
            str2 = Convert.ToString(numericUpDown2.Value);
            label5.Text = ("Введённое значение: " + str2 + " ед.");

            con2 = Convert.ToUInt16(numericUpDown2.Value);

            button2.Enabled = false;
        }

        private void button3_Click(object sender, EventArgs e)
        {
            str3 = Convert.ToString(numericUpDown3.Value);
            label6.Text = ("Введённое значение: " + str3 + " ед.");

            con3 = Convert.ToUInt16(numericUpDown3.Value);

            button3.Enabled = false;
        }

        private void button4_Click(object sender, EventArgs e)
        {
            label4.Text = " ";
            label5.Text = " ";
            label6.Text = " ";
            label7.Text = " ";
            label8.Text = " ";

            button1.Enabled = true;
            button2.Enabled = true;
            button3.Enabled = true;
            button5.Enabled = true;

            pictureBox2.Image = Properties.Resources._3092;
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if ((con1 == data1) && (con2 == data2))
            {
                label7.Text = "Наиболее верятное появление артефактов";
                label8.Text = "в аномалии №1";

                pictureBox2.Image = Properties.Resources._3091;
            }
            else
            {
                label7.Text = "Вероятность появления";
                label8.Text = "артефакта отсутствует";

                pictureBox2.Image = Properties.Resources._3093;
            }

            button5.Enabled = true;
        }

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            str1 = Convert.ToString(numericUpDown1.Value);
            label4.Text = ("Введённое значение: " + str1 + " ед.");

            button1.Enabled = false;

            con1 = Convert.ToUInt16(numericUpDown1.Value);
        }
    }
}
