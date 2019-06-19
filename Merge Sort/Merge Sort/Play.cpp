#include "Play.hpp"

void Play::sort(Play::Data data) {
	Rectangle* rectangle = data.rectangle;
	int rectangleNr = data.rectangleNr;
	bool* stop = data.stop;

	mergeSort(rectangle, 0, rectangleNr - 1);

	*stop = true;

	return;
}

void Play::merge(Rectangle* rectangle, int left, int pivot, int right) {

	int* extraArray = new int[(right - left + 1)];
	int i = left;
	int j = pivot + 1;
	int k = 0;


	while (i <= pivot && j <= right) {

		if ((rectangle + j)->getValue() < (rectangle + i)->getValue()) {

			*(extraArray + k) = (rectangle + j)->getValue();
			j++;
		}
		else{

			*(extraArray + k) = (rectangle + i)->getValue();
			i++;
		}
		k++;
	}

	if (i <= pivot){

		while (i <= pivot){

			*(extraArray + k) = (rectangle + i)->getValue();
			i++;
			k++;
		}
	}
	else{

		while (j <= right)
		{
			*(extraArray + k) = (rectangle + j)->getValue();
			j++;
			k++;
		}
	}

	for (i = 0; i <= right - left; i++) {
		(rectangle + left + i)->setValue(*(extraArray + i));
	}
	delete[] extraArray;
}

void Play::mergeSort(Rectangle* rectangle, int left, int right) {

	if (left < right) {
		int pivot = (left + right) / 2;
		mergeSort(rectangle, left, pivot);
		sleep(microseconds(20000));
		mergeSort(rectangle, pivot + 1, right);
		sleep(microseconds(20000));
		merge(rectangle, left, pivot, right);
	}
}

Play::Play(int size, string fontFile) {
	rectangleNr = size;
	rectangle = new Rectangle[size];

	start = false;
	stop = false;

	thread = new Thread{ Play::sort, Play::Data{rectangle, rectangleNr, &stop} };

	font.loadFromFile(fontFile);
	textClock.setFont(font);
	textClock.setCharacterSize(50);
	textClock.setFillColor(Color::White);
	textClock.setLetterSpacing(1);
	textClock.setStyle(Text::Bold);
	textClock.setPosition(Vector2f{ 500, 650 });

	reset();
}

Play::Data::Data(Rectangle* rectangle, int rectangleNr, bool* stop) {
	this->rectangle = rectangle;
	this->rectangleNr = rectangleNr;
	this->stop = stop;
}

Play::Type Play::run(RenderWindow& window) {

	if (delay.getElapsedTime().asSeconds() >= 0.1 && start && !stop) {

		int minutes = int(clock.getElapsedTime().asSeconds() / 60);
		string sMinutes = to_string(minutes);
		if (minutes < 10) sMinutes = "0" + sMinutes;

		int seconds = int(clock.getElapsedTime().asSeconds()) % 60;
		string sSeconds = to_string(seconds);
		if (seconds < 10) sSeconds = "0" + sSeconds;

		int miliseconds = int(int(clock.getElapsedTime().asMilliseconds()) % 100);
		string sMiliseconds = to_string(miliseconds);

		textClock.setString("Clock: " + sMinutes + " : " + sSeconds + " : " + sMiliseconds);
		delay.restart();
	}

	if (Mouse::isButtonPressed(Mouse::Left) && !start && !stop) {
		Vector2i vec = Mouse::getPosition(window);
		//
		if (vec.y < 550 && vec.y > 0 && vec.x >= 20 && vec.x < 1280) {
			vec.x -= 20;
			int nr = int(vec.x / 7);
			if (nr >= 0 && nr < rectangleNr) {
				(rectangle + nr)->setValue(550 - vec.y);
			}
		}
		//
	}

	if (Keyboard::isKeyPressed(Keyboard::Space) && !start) {
		start = true;
		clock.restart();
	}

	if (Keyboard::isKeyPressed(Keyboard::R)) {
		reset();
	}

	if (start && !stop && !on) {
		on = true;

		thread->launch();
	}

	if (Keyboard::isKeyPressed(Keyboard::Escape)) return Type::Return;

	return Type::None;
}

void Play::draw(RenderWindow& window) {
	for (int i = 0; i < rectangleNr; i++) {
		(rectangle + i)->draw(window, i);
	}
	window.draw(textClock);
}

void Play::reset() {
	thread->terminate();
	for (int i = 0; i < rectangleNr; i++) {
		int a = rand() % 500 + 1;
		(rectangle + i)->setValue(a);
		(rectangle + i)->setFillColor(Color::Red);
	}

	start = false;
	stop = false;
	on = false;

	delay.restart();
	textClock.setString("Clock: 00 : 00 : 0");
}

Play::~Play() {
	thread->terminate();
	delete thread;
	delete[] rectangle;
}