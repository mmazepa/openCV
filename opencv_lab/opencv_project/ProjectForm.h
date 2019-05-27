#pragma once

#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include<iostream>
#include<conio.h>

namespace opencvproject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	
	using namespace cv;
	using namespace std;

	VideoCapture cap;
	Mat frame, gray;

	int choice = 1;

	cv::String path = "M:/Programy/OpenCV/opencv/sources/data/haarcascades/";
	CascadeClassifier face_cascade = CascadeClassifier(path + "haarcascade_frontalface_default.xml");

	double scaleFactor = 1.5;
	int minNeighbors = 3;
	int minSize = 50;

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
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown3 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->label1 = (gcnew System::Windows::Forms::Label());
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
			this->button1->Location = System::Drawing::Point(16, 570);
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
			this->pictureBox1->Location = System::Drawing::Point(15, 94);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(640, 480);
			this->pictureBox1->TabIndex = 3;
			this->pictureBox1->TabStop = false;
			// 
			// timer1
			// 
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &ProjectForm::timer1_Tick);
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::Color::Transparent;
			this->button2->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"button2.BackgroundImage")));
			this->button2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->button2->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button2->FlatAppearance->BorderSize = 0;
			this->button2->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->button2->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->button2->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
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
			this->button3->BackColor = System::Drawing::Color::Transparent;
			this->button3->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"button3.BackgroundImage")));
			this->button3->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->button3->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button3->FlatAppearance->BorderSize = 0;
			this->button3->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->button3->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->button3->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button3->Location = System::Drawing::Point(161, 10);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(150, 25);
			this->button3->TabIndex = 5;
			this->button3->Text = L"Blur";
			this->button3->UseVisualStyleBackColor = false;
			this->button3->Click += gcnew System::EventHandler(this, &ProjectForm::button3_Click);
			// 
			// button4
			// 
			this->button4->BackColor = System::Drawing::Color::Transparent;
			this->button4->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"button4.BackgroundImage")));
			this->button4->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->button4->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button4->FlatAppearance->BorderSize = 0;
			this->button4->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->button4->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->button4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button4->Location = System::Drawing::Point(329, 10);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(150, 25);
			this->button4->TabIndex = 6;
			this->button4->Text = L"Black Rectangle";
			this->button4->UseVisualStyleBackColor = false;
			this->button4->Click += gcnew System::EventHandler(this, &ProjectForm::button4_Click);
			// 
			// button5
			// 
			this->button5->BackColor = System::Drawing::Color::Transparent;
			this->button5->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"button5.BackgroundImage")));
			this->button5->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->button5->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button5->FlatAppearance->BorderSize = 0;
			this->button5->FlatAppearance->MouseDownBackColor = System::Drawing::Color::Transparent;
			this->button5->FlatAppearance->MouseOverBackColor = System::Drawing::Color::Transparent;
			this->button5->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button5->Location = System::Drawing::Point(485, 10);
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
			this->panel1->BackColor = System::Drawing::SystemColors::ButtonFace;
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
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
			this->panel1->Location = System::Drawing::Point(15, 13);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(640, 70);
			this->panel1->TabIndex = 9;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(355, 43);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(43, 13);
			this->label3->TabIndex = 13;
			this->label3->Text = L"minSize";
			// 
			// numericUpDown3
			// 
			this->numericUpDown3->Location = System::Drawing::Point(404, 41);
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
			this->label2->Size = System::Drawing::Size(71, 13);
			this->label2->TabIndex = 11;
			this->label2->Text = L"minNeighbors";
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->Location = System::Drawing::Point(236, 41);
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
			this->label1->Size = System::Drawing::Size(62, 13);
			this->label1->TabIndex = 9;
			this->label1->Text = L"scaleFactor";
			// 
			// ProjectForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->ClientSize = System::Drawing::Size(669, 611);
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
	button2->BackgroundImage = System::Drawing::Image::FromFile("C:/Users/Mariusz/Desktop/opencv_tmp/projekt/guziki/button_purple.png");
}
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
	if (button1->Text == L"START") {
		timer1->Start();
		if (!cap.isOpened()) cap.open(0);
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
		cvtColor(frame, gray, CV_RGB2GRAY);
		equalizeHist(gray, gray);

		vector<Rect> faces;
		cv::Size newMinSize = cv::Size(minSize, minSize);

		face_cascade.detectMultiScale(gray, faces, scaleFactor, minNeighbors, 0, newMinSize);

		for (int i = 0; i < faces.size(); i++) {
			int face_radius = cvRound((faces[i].width + faces[i].height)*0.25);

			if (choice == 1) {
				cv::Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
				circle(frame, center, face_radius, Scalar(0, 100, 0), 3);
			}

			if (choice == 2)
				GaussianBlur(frame(faces[i]), frame(faces[i]), cv::Size(0, 0), 10);

			if (choice == 3) {
				int y = faces[i].y + face_radius/2;
				int height = faces[i].height * 0.3;
				Rect censored_black = Rect(faces[i].x, y, faces[i].width, height);
				rectangle(frame, censored_black, Scalar(0, 0, 0), CV_FILLED);
			}

			if (choice == 4) {
				Rect firstFace;
				if (faces.size() > 0) firstFace = faces[0];

				if (faces.size() > 1 && !firstFace.empty()) {
					Mat face = frame(firstFace);
					if (i > 0 && !face.empty()) {
						resize(face, face, cv::Size(faces[i].width, faces[i].height), 0, 0, INTER_CUBIC);
						face.copyTo(frame(faces[i]));
					}
				}
			}
		}

		if (!frame.empty()) {
			pictureBox1->Image = gcnew System::Drawing::Bitmap(frame.cols, frame.rows, frame.step, System::Drawing::Imaging::PixelFormat::Format24bppRgb, (System::IntPtr) frame.data);
			pictureBox1->Refresh();
		}
	} catch (cv::Exception e) {
		cap.open(1);
	}
}
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
	choice = 1;
	button2->BackgroundImage = System::Drawing::Image::FromFile("C:/Users/Mariusz/Desktop/opencv_tmp/projekt/guziki/button_purple.png");
	button3->BackgroundImage = System::Drawing::Image::FromFile("C:/Users/Mariusz/Desktop/opencv_tmp/projekt/guziki/button_green.png");
	button4->BackgroundImage = System::Drawing::Image::FromFile("C:/Users/Mariusz/Desktop/opencv_tmp/projekt/guziki/button_green.png");
	button5->BackgroundImage = System::Drawing::Image::FromFile("C:/Users/Mariusz/Desktop/opencv_tmp/projekt/guziki/button_green.png");
}
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
	choice = 2;
	button2->BackgroundImage = System::Drawing::Image::FromFile("C:/Users/Mariusz/Desktop/opencv_tmp/projekt/guziki/button_green.png");
	button3->BackgroundImage = System::Drawing::Image::FromFile("C:/Users/Mariusz/Desktop/opencv_tmp/projekt/guziki/button_purple.png");	
	button4->BackgroundImage = System::Drawing::Image::FromFile("C:/Users/Mariusz/Desktop/opencv_tmp/projekt/guziki/button_green.png");
	button5->BackgroundImage = System::Drawing::Image::FromFile("C:/Users/Mariusz/Desktop/opencv_tmp/projekt/guziki/button_green.png");
}
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
	choice = 3;
	button2->BackgroundImage = System::Drawing::Image::FromFile("C:/Users/Mariusz/Desktop/opencv_tmp/projekt/guziki/button_green.png");
	button3->BackgroundImage = System::Drawing::Image::FromFile("C:/Users/Mariusz/Desktop/opencv_tmp/projekt/guziki/button_green.png");
	button4->BackgroundImage = System::Drawing::Image::FromFile("C:/Users/Mariusz/Desktop/opencv_tmp/projekt/guziki/button_purple.png");
	button5->BackgroundImage = System::Drawing::Image::FromFile("C:/Users/Mariusz/Desktop/opencv_tmp/projekt/guziki/button_green.png");
}
private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
	choice = 4;
	button2->BackgroundImage = System::Drawing::Image::FromFile("C:/Users/Mariusz/Desktop/opencv_tmp/projekt/guziki/button_green.png");
	button3->BackgroundImage = System::Drawing::Image::FromFile("C:/Users/Mariusz/Desktop/opencv_tmp/projekt/guziki/button_green.png");
	button4->BackgroundImage = System::Drawing::Image::FromFile("C:/Users/Mariusz/Desktop/opencv_tmp/projekt/guziki/button_green.png");	
	button5->BackgroundImage = System::Drawing::Image::FromFile("C:/Users/Mariusz/Desktop/opencv_tmp/projekt/guziki/button_purple.png");
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
};
}
