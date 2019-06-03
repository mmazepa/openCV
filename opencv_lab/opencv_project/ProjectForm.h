#pragma once

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include<iostream>
#include<conio.h>
#include<msclr\marshal_cppstd.h>

namespace opencvproject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	
	using namespace System::IO;

	using namespace cv;
	using namespace std;

	VideoCapture cap;
	Mat frame, gray;
	Mat modFrame;

	int choice = 1;

	cv::String path = "M:/Programy/OpenCV/opencv/sources/data/haarcascades/";
	CascadeClassifier face_cascade = CascadeClassifier(path + "haarcascade_frontalface_default.xml");

	double scaleFactor = 1.5;
	int minNeighbors = 3;
	int minSize = 50;

	string facePath = "";
	VideoWriter outputVideo;

	int mouseX = 0;
	int mouseY = 0;

	vector<Rect> faces;
	int faceCounter = 1;

	public ref class ProjectForm : public System::Windows::Forms::Form
	{
	public:
		ProjectForm(void)
		{
			InitializeComponent();
		}

	protected:
		~ProjectForm()
		{
			if (components)
			{
				delete components;
			}
		}

	
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::NumericUpDown^  numericUpDown3;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Button^  button8;
	private: System::Windows::Forms::RadioButton^  radioButton2;
	private: System::Windows::Forms::RadioButton^  radioButton1;

	private: System::ComponentModel::IContainer^  components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(ProjectForm::typeid));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown3 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button8 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->BeginInit();
			this->panel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown2))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::Transparent;
			this->button1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"button1.BackgroundImage")));
			this->button1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->button1->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button1->FlatAppearance->BorderSize = 0;
			this->button1->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->button1->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->button1->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->button1->Location = System::Drawing::Point(15, 620);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(639, 43);
			this->button1->TabIndex = 1;
			this->button1->Text = L"START";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &ProjectForm::button1_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"pictureBox1.BackgroundImage")));
			this->pictureBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pictureBox1->Location = System::Drawing::Point(15, 140);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(640, 480);
			this->pictureBox1->TabIndex = 3;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &ProjectForm::pictureBox1_MouseDown);
			// 
			// timer1
			// 
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &ProjectForm::timer1_Tick);
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::Color::Gray;
			this->button2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->button2->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button2->FlatAppearance->BorderSize = 0;
			this->button2->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->button2->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->button2->Location = System::Drawing::Point(5, 10);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(150, 25);
			this->button2->TabIndex = 4;
			this->button2->Text = L"Standard";
			this->button2->UseVisualStyleBackColor = false;
			this->button2->Click += gcnew System::EventHandler(this, &ProjectForm::button2_Click);
			// 
			// button3
			// 
			this->button3->BackColor = System::Drawing::Color::Gray;
			this->button3->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->button3->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button3->FlatAppearance->BorderSize = 0;
			this->button3->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->button3->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->button3->Location = System::Drawing::Point(163, 10);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(150, 25);
			this->button3->TabIndex = 5;
			this->button3->Text = L"Blur";
			this->button3->UseVisualStyleBackColor = false;
			this->button3->Click += gcnew System::EventHandler(this, &ProjectForm::button3_Click);
			// 
			// button4
			// 
			this->button4->BackColor = System::Drawing::Color::Gray;
			this->button4->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->button4->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button4->FlatAppearance->BorderSize = 0;
			this->button4->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->button4->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->button4->Location = System::Drawing::Point(321, 10);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(150, 25);
			this->button4->TabIndex = 6;
			this->button4->Text = L"Black Rectangle";
			this->button4->UseVisualStyleBackColor = false;
			this->button4->Click += gcnew System::EventHandler(this, &ProjectForm::button4_Click);
			// 
			// button5
			// 
			this->button5->BackColor = System::Drawing::Color::Gray;
			this->button5->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->button5->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button5->FlatAppearance->BorderSize = 0;
			this->button5->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->button5->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->button5->Location = System::Drawing::Point(479, 10);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(150, 25);
			this->button5->TabIndex = 7;
			this->button5->Text = L"Face Copying";
			this->button5->UseVisualStyleBackColor = false;
			this->button5->Click += gcnew System::EventHandler(this, &ProjectForm::button5_Click);
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->DecimalPlaces = 2;
			this->numericUpDown1->Increment = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 131072 });
			this->numericUpDown1->Location = System::Drawing::Point(80, 41);
			this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 5, 0, 0, 0 });
			this->numericUpDown1->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 11, 0, 0, 65536 });
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(75, 20);
			this->numericUpDown1->TabIndex = 8;
			this->numericUpDown1->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 15, 0, 0, 65536 });
			this->numericUpDown1->ValueChanged += gcnew System::EventHandler(this, &ProjectForm::numericUpDown1_ValueChanged);
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::Transparent;
			this->panel1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"panel1.BackgroundImage")));
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->panel1->Controls->Add(this->radioButton2);
			this->panel1->Controls->Add(this->radioButton1);
			this->panel1->Controls->Add(this->button7);
			this->panel1->Controls->Add(this->label4);
			this->panel1->Controls->Add(this->textBox1);
			this->panel1->Controls->Add(this->button6);
			this->panel1->Controls->Add(this->label3);
			this->panel1->Controls->Add(this->numericUpDown3);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->numericUpDown2);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->button2);
			this->panel1->Controls->Add(this->numericUpDown1);
			this->panel1->Controls->Add(this->button5);
			this->panel1->Controls->Add(this->button3);
			this->panel1->Controls->Add(this->button4);
			this->panel1->Location = System::Drawing::Point(15, 15);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(640, 110);
			this->panel1->TabIndex = 9;
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(15, 86);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(52, 17);
			this->radioButton2->TabIndex = 19;
			this->radioButton2->Text = L"Video";
			this->radioButton2->UseVisualStyleBackColor = true;
			this->radioButton2->CheckedChanged += gcnew System::EventHandler(this, &ProjectForm::radioButton2_CheckedChanged);
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Checked = true;
			this->radioButton1->Location = System::Drawing::Point(15, 67);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(61, 17);
			this->radioButton1->TabIndex = 18;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"Camera";
			this->radioButton1->UseVisualStyleBackColor = true;
			this->radioButton1->CheckedChanged += gcnew System::EventHandler(this, &ProjectForm::radioButton1_CheckedChanged);
			// 
			// button7
			// 
			this->button7->BackColor = System::Drawing::Color::Plum;
			this->button7->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button7->Location = System::Drawing::Point(479, 74);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(150, 23);
			this->button7->TabIndex = 17;
			this->button7->Text = L"Browse...";
			this->button7->UseVisualStyleBackColor = false;
			this->button7->Click += gcnew System::EventHandler(this, &ProjectForm::button7_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(84, 77);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(63, 13);
			this->label4->TabIndex = 16;
			this->label4->Text = L"Path to File:";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(149, 74);
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(322, 20);
			this->textBox1->TabIndex = 15;
			// 
			// button6
			// 
			this->button6->BackColor = System::Drawing::Color::LightSalmon;
			this->button6->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button6->Location = System::Drawing::Point(479, 38);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(150, 23);
			this->button6->TabIndex = 14;
			this->button6->Text = L"Face From File";
			this->button6->UseVisualStyleBackColor = false;
			this->button6->Click += gcnew System::EventHandler(this, &ProjectForm::button6_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(347, 43);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(46, 13);
			this->label3->TabIndex = 13;
			this->label3->Text = L"minSize:";
			// 
			// numericUpDown3
			// 
			this->numericUpDown3->Location = System::Drawing::Point(396, 41);
			this->numericUpDown3->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 250, 0, 0, 0 });
			this->numericUpDown3->Name = L"numericUpDown3";
			this->numericUpDown3->Size = System::Drawing::Size(75, 20);
			this->numericUpDown3->TabIndex = 12;
			this->numericUpDown3->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 50, 0, 0, 0 });
			this->numericUpDown3->ValueChanged += gcnew System::EventHandler(this, &ProjectForm::numericUpDown3_ValueChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(161, 43);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(74, 13);
			this->label2->TabIndex = 11;
			this->label2->Text = L"minNeighbors:";
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->Location = System::Drawing::Point(238, 41);
			this->numericUpDown2->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10, 0, 0, 0 });
			this->numericUpDown2->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->numericUpDown2->Name = L"numericUpDown2";
			this->numericUpDown2->Size = System::Drawing::Size(75, 20);
			this->numericUpDown2->TabIndex = 10;
			this->numericUpDown2->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3, 0, 0, 0 });
			this->numericUpDown2->ValueChanged += gcnew System::EventHandler(this, &ProjectForm::numericUpDown2_ValueChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 43);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(65, 13);
			this->label1->TabIndex = 9;
			this->label1->Text = L"scaleFactor:";
			// 
			// button8
			// 
			this->button8->BackColor = System::Drawing::Color::CornflowerBlue;
			this->button8->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button8->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button8->Location = System::Drawing::Point(25, 150);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(100, 25);
			this->button8->TabIndex = 10;
			this->button8->Text = L"Record Video";
			this->button8->UseVisualStyleBackColor = false;
			this->button8->Click += gcnew System::EventHandler(this, &ProjectForm::button8_Click);
			// 
			// ProjectForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->ClientSize = System::Drawing::Size(669, 661);
			this->Controls->Add(this->button8);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->button1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Name = L"ProjectForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Face Detection Project 2019";
			this->Load += gcnew System::EventHandler(this, &ProjectForm::ProjectForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->EndInit();
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown2))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void ProjectForm_Load(System::Object^  sender, System::EventArgs^  e) {
	this->button2->BackColor = System::Drawing::Color::Chartreuse;

	this->button6->BackColor = System::Drawing::Color::Gray;
	this->button6->Enabled = false;

	this->button7->BackColor = System::Drawing::Color::Gray;
	this->button7->Enabled = false;
}
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
	if (button1->Text == L"START") {
		timer1->Start();
		if (!cap.isOpened()) {
			if (!radioButton1->Checked == true)
				radioButton1->Checked = true;
			cap.open(0);
		}
		button1->Text = L"STOP";
	} else if (button1->Text == L"STOP") {
		timer1->Stop();
		cap.release();
		frame.release();
		pictureBox1->Image = nullptr;
		button1->Text = L"START";
	}
}
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
	try {
		cap >> frame;
		if (frame.empty()) {
			cap.set(CAP_PROP_POS_FRAMES, 0);
			cap >> frame;
		}
		modFrame = frame.clone();
		cvtColor(frame, gray, CV_RGB2GRAY);
		equalizeHist(gray, gray);

		cv::Size newMinSize = cv::Size(minSize, minSize);

		face_cascade.detectMultiScale(gray, faces, scaleFactor, minNeighbors, 0, newMinSize);

		for (int i = 0; i < faces.size(); i++) {
			int face_radius = cvRound((faces[i].width + faces[i].height)*0.25);

			if (choice == 1) {
				cv::Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
				circle(modFrame, center, face_radius, Scalar(0, 100, 0), 3);
			}

			if (choice == 2)
				GaussianBlur(modFrame(faces[i]), modFrame(faces[i]), cv::Size(0, 0), 10);

			if (choice == 3) {
				int y = faces[i].y + face_radius/2;
				int height = faces[i].height * 0.3;
				Rect censored_black = Rect(faces[i].x, y, faces[i].width, height);
				rectangle(modFrame, censored_black, Scalar(0, 0, 0), CV_FILLED);
			}

			if (choice == 4 || choice == 5) {
				Mat face;
				
				if (faces.size() > 0) {
					if (choice == 4) {
						if (mouseX == 0 && mouseY == 0) {
							Rect firstFace = faces[0];
							face = modFrame(firstFace);
						} else {
							for (Rect aface : faces) {
								if (mouseX >= aface.x && mouseY >= aface.y) {
									if (mouseX <= aface.x + aface.width && mouseY <= aface.y + aface.height) {
										face = modFrame(aface);
									}
								}
							}
						}
					}
					
					if (choice == 5) {
						face = imread(facePath);
					}

					if (!face.empty()) {
						resize(face, face, cv::Size(faces[i].width, faces[i].height), 0, 0, INTER_CUBIC);
						face.copyTo(modFrame(faces[i]));
					}
				}
			}
		}

		if (!modFrame.empty()) {
			pictureBox1->Image = gcnew System::Drawing::Bitmap(modFrame.cols, modFrame.rows, modFrame.step, System::Drawing::Imaging::PixelFormat::Format24bppRgb, (System::IntPtr) modFrame.data);
			pictureBox1->Refresh();
			if (outputVideo.isOpened()) {
				outputVideo.write(modFrame);
			}
		}
	} catch (cv::Exception e) {
		cap.open(1);
	}
}
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
	choice = 1;
	this->button2->BackColor = System::Drawing::Color::Chartreuse;
	this->button3->BackColor = System::Drawing::Color::Gray;
	this->button4->BackColor = System::Drawing::Color::Gray;
	this->button5->BackColor = System::Drawing::Color::Gray;

	this->button6->BackColor = System::Drawing::Color::Gray;
	this->button6->Enabled = false;

	this->button7->BackColor = System::Drawing::Color::Gray;
	this->button7->Enabled = false;
}
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
	choice = 2;
	this->button2->BackColor = System::Drawing::Color::Gray;
	this->button3->BackColor = System::Drawing::Color::Chartreuse;
	this->button4->BackColor = System::Drawing::Color::Gray;
	this->button5->BackColor = System::Drawing::Color::Gray;

	this->button6->BackColor = System::Drawing::Color::Gray;
	this->button6->Enabled = false;

	this->button7->BackColor = System::Drawing::Color::Gray;
	this->button7->Enabled = false;
}
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
	choice = 3;
	this->button2->BackColor = System::Drawing::Color::Gray;
	this->button3->BackColor = System::Drawing::Color::Gray;
	this->button4->BackColor = System::Drawing::Color::Chartreuse;
	this->button5->BackColor = System::Drawing::Color::Gray;

	this->button6->BackColor = System::Drawing::Color::Gray;
	this->button6->Enabled = false;

	this->button7->BackColor = System::Drawing::Color::Gray;
	this->button7->Enabled = false;
}
private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
	choice = 4;
	this->button2->BackColor = System::Drawing::Color::Gray;
	this->button3->BackColor = System::Drawing::Color::Gray;
	this->button4->BackColor = System::Drawing::Color::Gray;
	this->button5->BackColor = System::Drawing::Color::Chartreuse;

	this->button6->BackColor = System::Drawing::Color::Gray;
	this->button6->Enabled = true;

	this->button7->BackColor = System::Drawing::Color::Plum;
	this->button7->Enabled = true;
}
private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {
	choice = 5;
	this->button2->BackColor = System::Drawing::Color::Gray;
	this->button3->BackColor = System::Drawing::Color::Gray;
	this->button4->BackColor = System::Drawing::Color::Gray;
	this->button5->BackColor = System::Drawing::Color::Gray;

	this->button6->BackColor = System::Drawing::Color::Chartreuse;
}
private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e) {
	OpenFileDialog^ openFileDialog = gcnew OpenFileDialog;
	
	openFileDialog->Filter = "Image files (*.jpg;*.png;*.bmp) | *.jpg;*.png;*.bmp | All files | *.*";
	openFileDialog->FilterIndex = 1;
	openFileDialog->RestoreDirectory = true;

	if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		this->textBox1->Text = openFileDialog->FileName;
		msclr::interop::marshal_context context;
		facePath = context.marshal_as<std::string>(openFileDialog->FileName);

		this->textBox1->SelectionStart = this->textBox1->Text->Length;
		this->textBox1->SelectionLength = 0;
	}
}
private: System::Void button8_Click(System::Object^  sender, System::EventArgs^  e) {
	if (button8->Text == "Record Video") {
		this->button8->Text = "Stop";

		string path = "C:/Users/Mariusz/Desktop/opencv_tmp/projekt/nagrania/";
		string outputVideoName = "recorded_sample.avi";
		string outputPath = path + outputVideoName;

		int codec = CV_FOURCC('M', 'J', 'P', 'G');
		cv::Size size = cv::Size(640, 480);
		
		outputVideo.open(outputPath, codec, 15, size, true);
	} else if (button8->Text == "Stop") {
		this->button8->Text = "Record Video";
		outputVideo.release();
	}
}
private: System::Void numericUpDown1_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	scaleFactor = (double) numericUpDown1->Value;
}
private: System::Void numericUpDown2_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	minNeighbors = (int) numericUpDown2->Value;
}
private: System::Void numericUpDown3_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	minSize = (int) numericUpDown3->Value;
}
private: System::Void pictureBox1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
	System::Drawing::Point point = e->Location;
	mouseX = point.X;
	mouseY = point.Y;

	if (choice == 4) {
		string path = "C:/Users/Mariusz/Desktop/opencv_tmp/projekt/twarze/";
		for (Rect aface : faces) {
			if (mouseX >= aface.x && mouseY >= aface.y) {
				if (mouseX <= aface.x + aface.width && mouseY <= aface.y + aface.height) {
					string filename = "face_no-" + to_string(faceCounter) + ".jpg";
					imwrite(path + filename, frame(aface));
					faceCounter++;
				}
			}
		}
	}
}
private: System::Void radioButton1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	cap.release();
	cap.open(0);
}
private: System::Void radioButton2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
	OpenFileDialog^ openFileDialog = gcnew OpenFileDialog;

	if (this->radioButton2->Checked == true) {
		openFileDialog->Filter = "Video files (*.avi;*.mp4;) | *.avi;*.mp4; | All files | *.*";
		openFileDialog->FilterIndex = 1;
		openFileDialog->RestoreDirectory = true;

		string videoFilePath = "";
		if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			cap.release();
			msclr::interop::marshal_context context;
			videoFilePath = context.marshal_as<std::string>(openFileDialog->FileName);
			cap.open(videoFilePath);
		} else {
			this->radioButton2->Checked = false;
			this->radioButton1->Checked = true;
		}
	}
}
};
}
