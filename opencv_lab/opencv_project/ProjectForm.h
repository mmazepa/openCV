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

	// double...
	int scaleFactor = 15;
	int scaleFactor_max = 100;

	// int...
	int minNeighbors = 3;
	int minNeighbors_max = 10;

	// size...
	int minSize = 50;
	int minSize_max = 250;

	double prepareDoubleValue(int min, int max, int ratio) {
		double val = (double)min / max;
		return val * ratio;
	}

	double guardRange(double value) {
		if (value <= 1.0) value = 1.1;
		return value;
	}

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
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
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
			this->button1->Location = System::Drawing::Point(15, 521);
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
			this->pictureBox1->Location = System::Drawing::Point(15, 44);
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
			this->button2->Location = System::Drawing::Point(15, 12);
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
			this->button3->Location = System::Drawing::Point(177, 12);
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
			this->button4->Location = System::Drawing::Point(343, 12);
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
			this->button5->Location = System::Drawing::Point(505, 12);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(150, 25);
			this->button5->TabIndex = 7;
			this->button5->Text = L"Face Copying";
			this->button5->UseVisualStyleBackColor = false;
			this->button5->Click += gcnew System::EventHandler(this, &ProjectForm::button5_Click);
			// 
			// ProjectForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"$this.BackgroundImage")));
			this->ClientSize = System::Drawing::Size(669, 561);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->button1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Name = L"ProjectForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Face Detection Project 2019";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
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
		double newScaleFactor = guardRange(prepareDoubleValue(scaleFactor, scaleFactor_max, 10));
		cv::Size newMinSize = cv::Size(minSize, minSize);

		face_cascade.detectMultiScale(gray, faces, newScaleFactor, minNeighbors, 0, newMinSize);

		for (int i = 0; i < faces.size(); i++) {
			if (choice == 1) {
				cv::Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
				int face_radius = cvRound((faces[i].width + faces[i].height)*0.25);
				circle(frame, center, face_radius, Scalar(0, 100, 0), 3);
			}

			if (choice == 2)
				GaussianBlur(frame(faces[i]), frame(faces[i]), cv::Size(0, 0), 10);

			if (choice == 3) {
				int y = faces[i].y * 1.3;
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
}
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
	choice = 2;
}
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
	choice = 3;
}
private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
	choice = 4;
}
};
}
