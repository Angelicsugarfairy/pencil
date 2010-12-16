/*

Pencil - Traditional Animation Software
Copyright (C) 2005-2007 Patrick Corrieri & Pascal Naidon

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation;

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/
#include <QtGui>
#include "toolset.h"
#include <math.h>

SpinSlider::SpinSlider(QString text, QString type, QString dataType, qreal min, qreal max, QWidget* parent) : QWidget(parent) {
	value = 1.0;
	this->type = type;
	this->dataType = dataType;
	this->min = min;
	this->max = max;
	QLabel* label = new QLabel(text+": ");
	label->setFont( QFont("Helvetica", 10) );
	valueLabel = new QLabel("--");
	valueLabel->setFont( QFont("Helvetica", 10) );
        //valueLabel->setFixedWidth(30);
	valueLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
	slider = new QSlider(Qt::Horizontal, this);
	slider->setMinimum(0);
	slider->setMaximum(100);
	slider->setMaximumWidth(70);
				//slider->setFixedHeight(16);
	QGridLayout *lay = new QGridLayout();
	lay->setMargin(2);
	lay->setSpacing(2);
        //lay->setColumnStretch(0,1);
	lay->addWidget(label,0,0,1,1);
	lay->addWidget(valueLabel,0,1,1,1);
	lay->addWidget(slider,1,0,1,2);
	setLayout(lay);
        setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);

	connect(slider, SIGNAL(valueChanged(int)), this, SLOT(changeValue(int)));
	connect(slider, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()));
	connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(sliderMoved(int)));
}

void SpinSlider::changeValue(qreal value) {
	this->value = value;
	if(dataType == "integer") {
		valueLabel->setText( QString::number(qRound(value)) );
	} else {
		valueLabel->setText( QLocale::system().toString(value,'f',1) );
	}
}

void SpinSlider::changeValue(int value) {
	qreal value2 = 0.0;
	if(type=="linear") value2 = min + value*(max-min)/100;
	if(type=="log") value2 = min*exp( value*log(max/min)/100 );
	changeValue(value2);
}

void SpinSlider::setValue(qreal value) {
	qreal value2 = 0.0;
	if(type=="linear") value2 = qRound( 100*(value-min)/(max-min) );
	if(type=="log") value2 = qRound( 100.0*log(value/0.2)/log(1000.0) );
	slider->setSliderPosition(value2);
	changeValue(value);
}

void SpinSlider::sliderReleased() {
	//qDebug() << "sliderReleased";
	emit valueChanged(this->value);
}

void SpinSlider::sliderMoved(int value) {
	changeValue(value);
	emit valueChanged(this->value);
}

// ----------------------------------------------------------------------------------

ToolSet::ToolSet() {

	drawPalette = new QDockWidget(tr("Tools"));
	optionPalette = new QDockWidget(tr("Options"));
	displayPalette = new QDockWidget(tr("Display"));
	keyPalette = new QDockWidget(tr("Keys"));
	onionPalette = new QDockWidget(tr("Onion skin"));
	timePalette = new QDockWidget(tr("Controls"));

#ifdef Q_WS_MAC
	//drawPalette->setTitleBarWidget(new TitleBar(drawPalette));
#endif
	//drawPalette->setAutoFillBackground(false);
	//drawPalette->setAttribute(Qt::WA_NoSystemBackground, true);

	/*QPalette optionPalettePalette(optionPalette->palette());
	optionPalettePalette.setColor(QPalette::Light, Qt::red);
	optionPalettePalette.setColor(QPalette::Midlight, Qt::red);
	optionPalettePalette.setColor(QPalette::Dark, Qt::red);
	optionPalettePalette.setColor(QPalette::Shadow, Qt::red);
	optionPalette->setPalette(optionPalettePalette);*/

	//optionPalette->setWindowFlags(Qt::Tool);
	//optionPalette->setWindowIconText(tr("Coucou"));
	//optionPalette->setFont( QFont("Helvetica", 14, QFont::Bold) );

	//QGroupBox *drawGroup = new QGroupBox(tr("Draw"));
	QFrame *drawGroup = new QFrame();
	QFrame *optionGroup = new QFrame();
	QFrame *displayGroup = new QFrame();
	QFrame *keyGroup = new QFrame();
	QFrame *onionGroup = new QFrame();
	QFrame * timeGroup = new QFrame();

	//QFrame *line = new QFrame();
	//line->setFrameStyle(QFrame::HLine | QFrame::Raised);

	//drawGroup->setMaximumHeight( 100 );
	//optionGroup->setFrameStyle(QFrame::Panel | QFrame::Raised);
	//keyGroup->setFrameStyle(QFrame::Panel | QFrame::Raised);
	//onionGroup->setFrameStyle(QFrame::Panel | QFrame::Raised);
	//timeGroup->setFrameStyle(QFrame::Panel | QFrame::Raised);

	//optionGroup->setLineWidth(2);
	//keyGroup->setLineWidth(2);
	//displayGroup->setLineWidth(2);
	//onionGroup->setLineWidth(2);
	//timeGroup->setLineWidth(2);

	drawPalette->setWidget(drawGroup);
	optionPalette->setWidget(optionGroup);
	displayPalette->setWidget(displayGroup);
	keyPalette->setWidget(keyGroup);
	onionPalette->setWidget(onionGroup);
	timePalette->setWidget(timeGroup);

	//QVBoxLayout *framelay = new QVBoxLayout();
	QGridLayout *drawLay = new QGridLayout();
	QGridLayout *optionLay = new QGridLayout();
	QGridLayout *displayLay = new QGridLayout();
	QGridLayout *keyLay = new QGridLayout();
	QGridLayout *onionLay = new QGridLayout();
	QGridLayout *timeLay = new QGridLayout();
	//QVBoxLayout *butlay = new QVBoxLayout();

	QSettings settings("Pencil","Pencil");

	newToolButton(pencilButton);
	newToolButton(selectButton);
	newToolButton(moveButton);
	newToolButton(handButton);
	newToolButton(penButton);
	newToolButton(eraserButton);
	newToolButton(polylineButton);
	newToolButton(bucketButton);
	newToolButton(colouringButton);
	newToolButton(eyedropperButton);
	newToolButton(clearButton);
	newToolButton(magnifyButton);
	newToolButton(smudgeButton);

	add = new QToolButton(this);
	rm = new QToolButton(this);
	thinLinesButton = new QToolButton(this);
	outlinesButton = new QToolButton(this);
	mirrorButton = new QToolButton(this); //  = new QToolButton(this);
	mirrorButtonV = new QToolButton(this); //  = new QToolButton(this);
		onionPrev = new QToolButton(this);
		onionNext = new QToolButton(this);
	choseColour = new QToolButton(this);
	play = new QPushButton(tr("Play"),this);

	play->setFixedWidth(80);

	pencilButton->setWhatsThis("Pencil Tool (N)");
	selectButton->setWhatsThis("Select Tool (V)");
	moveButton->setWhatsThis("Move Tool (Q)");
	handButton->setWhatsThis("Hand Tool (H)");
	penButton->setWhatsThis("Pen Tool (P)");
	eraserButton->setWhatsThis("Eraser Tool (E)");
	polylineButton->setWhatsThis("Polyline Tool (Y)");
	bucketButton->setWhatsThis("Paint Bucket Tool(K)");
	colouringButton->setWhatsThis("Brush Tool(B)");
	eyedropperButton->setWhatsThis("Eyedropper Tool (I)");
	clearButton->setWhatsThis("Clear Tool");
	magnifyButton->setWhatsThis("Zoom Tool (Z)");
	smudgeButton->setWhatsThis("Smudge Tool (A)");

	usePressureBox = new QCheckBox("Pressure");
		pencilButton->setToolTip("Size with pressure");
			usePressureBox->setFont( QFont("Helvetica", 10) );
				usePressureBox->setChecked(true);
	makeInvisibleBox = new QCheckBox("Invisible");
		makeInvisibleBox->setToolTip("Make invisible");
			makeInvisibleBox->setFont( QFont("Helvetica", 10) );
				makeInvisibleBox->setChecked(false);
	preserveAlphaBox = new QCheckBox("Alpha");
		preserveAlphaBox->setToolTip("Preserve Alpha");
			preserveAlphaBox->setFont( QFont("Helvetica", 10) );
				preserveAlphaBox->setChecked(false);
	followContourBox = new QCheckBox("Contours");
		followContourBox->setToolTip("Stop at contours");
			followContourBox->setFont( QFont("Helvetica", 10) );
				followContourBox->setChecked(false);
	// -- mj
	//onionPrevBox = new QCheckBox("Previous");
	//		onionPrevBox->setFont( QFont("Helvetica", 10) );
	//			onionPrevBox->setChecked(true);
	//onionNextBox = new QCheckBox("Next");
	//		onionNextBox->setFont( QFont("Helvetica", 10) );


	/*sizeSlider = new QDoubleSpinBox(this);
	sizeSlider->setFocusPolicy(Qt::NoFocus);
	sizeSlider->setFont( QFont("Helvetica", 10) );
	sizeSlider->setDecimals(1);
	sizeSlider->setMinimum(0);
	sizeSlider->setMaximum(200);
	sizeSlider->setValue(settings.value("width").toInt());
	sizeSlider->setFixedWidth(50);
	sizeSlider->setFixedHeight(22);*/

	sizeSlider = new SpinSlider("Size", "log", "real", 0.2, 200.0, this);
	sizeSlider->setValue(settings.value("pencilWidth").toDouble());

	featherSlider = new SpinSlider("Feather", "log", "real", 0.2, 200.0, this);
	featherSlider->setValue(settings.value("pencilFeather").toDouble());

	opacitySlider = new SpinSlider("Opacity", "linear", "real", 0.0, 1.0, this);
	opacitySlider->setValue(settings.value("pencilOpacity").toDouble());

	/*featherSlider = new QSlider(Qt::Horizontal, this);
	featherSlider->setMinimum(0);
	featherSlider->setMaximum(100);
	featherSlider->setFixedHeight(22);*/

	soundBox = new QCheckBox("Sound");
	soundBox->setFont( QFont("Helvetica", 10) );
	loopBox = new QCheckBox(tr("Loop"), this);
	loopBox->setFont( QFont("Helvetica", 10) );

	fpsBox = new QSpinBox(this);
	//fpsBox->setFixedWidth(50);
	fpsBox->setFont( QFont("Helvetica", 10) );
	fpsBox->setFixedHeight(22);
	fpsBox->setValue(settings.value("fps").toInt());
	fpsBox->setMinimum(1);
	fpsBox->setMaximum(50);
	fpsBox->setToolTip("FPS");
	fpsBox->setFocusPolicy(Qt::NoFocus);

	/*framecounter = new QLabel;
	framecounter->setText("1");
	framecounter->setAlignment(Qt::AlignCenter);
	framecounter->setToolTip("Frame Counter!");
	framecounter->setFont( QFont("Helvetica", 40, QFont::Bold) );*/

	// -- mj
	//onionNextBox->setToolTip("Onion skin next frame");
	//onionPrevBox->setToolTip("Onion skin previous frame");
	sizeSlider->setToolTip("Set Pen Width");

		pencilButton->setIcon(QIcon(":icons/pencil2.png"));
			pencilButton->setToolTip("Pencil Tool <b>(N)</b>: Sketch with pencil");
		selectButton->setIcon(QIcon(":icons/select.png"));
			selectButton->setToolTip("Select Tool <b>(V)</b>: Select an object");
		moveButton->setIcon(QIcon(":icons/arrow.png"));
			moveButton->setToolTip("Move Tool <b>(Q)</b>: Move an object");
		handButton->setIcon(QIcon(":icons/hand.png"));
			handButton->setToolTip("Hand Tool <b>(H)</b>: Move the canvas");
		penButton->setIcon(QIcon(":icons/pen.png"));
			penButton->setToolTip("Pen Tool <b>(P)</b>: Sketch with pen");
		eraserButton->setIcon(QIcon(":icons/eraser.png"));
			eraserButton->setToolTip("Eraser Tool <b>(E)</b>: Erase");
		polylineButton->setIcon(QIcon(":icons/polyline.png"));
			polylineButton->setToolTip("Polyline Tool <b>(Y)</b>: Create line/curves");
		bucketButton->setIcon(QIcon(":icons/bucket.png"));
			bucketButton->setToolTip("Paint Bucket Tool <b>(K)</b>: Fill selected area with a color");
		colouringButton->setIcon(QIcon(":icons/brush.png"));
			colouringButton->setToolTip("Brush Tool <b>(B)</b>: Paint smooth stroke with a brush");
		eyedropperButton->setIcon(QIcon(":icons/eyedropper.png"));
			eyedropperButton->setToolTip("Eyedropper Tool <b>(I)</b>: Set color from the stage");
		magnifyButton->setIcon(QIcon(":icons/magnify.png"));
			magnifyButton->setToolTip("Zoom Tool <b>(Z)</b>: Adjust the zoom level");
				magnifyButton->setEnabled(false);
		smudgeButton->setIcon(QIcon(":icons/smudge.png"));
			smudgeButton->setToolTip("Smudge Tool <b>(A)</b>: Edit polyline/curves");
			smudgeButton->setEnabled(true);

	add->setIcon(QIcon(":icons/add.png"));
	add->setToolTip("Add Frame");
	rm->setIcon(QIcon(":icons/remove.png"));
	rm->setToolTip("Remove Frame");

	QPixmap colourSwatch(30,30);
	colourSwatch.fill(Qt::black);
	QLabel* colourLabel = new QLabel();
	colourLabel->setText(tr("Color:"));
	colourLabel->setFont( QFont("Helvetica", 10) );
	choseColour->setIcon(colourSwatch);
	choseColour->setToolTip("Display Colors");

	thinLinesButton->setIcon(QIcon(":icons/thinlines5.png"));
	thinLinesButton->setToolTip("Show invisible lines");
	outlinesButton->setIcon(QIcon(":icons/outlines5.png"));
	outlinesButton->setToolTip("Show outlines only");

	mirrorButton->setIcon(QIcon(":icons/mirror.png"));
	mirrorButton->setToolTip("Horizontal flip");
	mirrorButtonV->setIcon(QIcon(":icons/mirrorV.png"));
			mirrorButtonV->setToolTip("Horizontal flip");

			onionPrev->setIcon(QIcon(":icons/onionPrev.png"));
			onionPrev->setToolTip("Onion skin previous frame");
			onionNext->setIcon(QIcon(":icons/onionNext.png"));
			onionNext->setToolTip("Onion skin next frame");
	clearButton->setIcon(QIcon(":icons/clear.png"));
	clearButton->setToolTip("Clear Tool <b>(L)</b>: Erases content of selected frame");
	thinLinesButton->setCheckable(true);
	thinLinesButton->setChecked(false);
	outlinesButton->setCheckable(true);
	outlinesButton->setChecked(false);
	mirrorButton->setCheckable(true);
	mirrorButton->setChecked(false);
	mirrorButtonV->setCheckable(true);
		mirrorButtonV->setChecked(false);
		onionPrev->setCheckable(true);
		onionPrev->setChecked(false);
		onionNext->setCheckable(true);
		onionNext->setChecked(false);
	pencilButton->setCheckable(true);
	penButton->setCheckable(true);
	polylineButton->setCheckable(true);
	bucketButton->setCheckable(true);
	colouringButton->setCheckable(true);
	smudgeButton->setCheckable(true);
	eyedropperButton->setCheckable(true);
	selectButton->setCheckable(true);
	moveButton->setCheckable(true);
	handButton->setCheckable(true);
	eraserButton->setCheckable(true);
	pencilButton->setChecked(true);

	drawLay->setMargin(2);
	drawLay->setSpacing(0);

	drawLay->addWidget(moveButton,0,0); drawLay->setAlignment(moveButton, Qt::AlignRight);
	drawLay->addWidget(clearButton,0,1); drawLay->setAlignment(clearButton, Qt::AlignLeft);

	drawLay->addWidget(selectButton,1,0); drawLay->setAlignment(selectButton, Qt::AlignRight);
	drawLay->addWidget(colouringButton,1,1); drawLay->setAlignment(colouringButton, Qt::AlignLeft);

	drawLay->addWidget(polylineButton,2,0); drawLay->setAlignment(polylineButton, Qt::AlignRight);
	drawLay->addWidget(smudgeButton,2,1); drawLay->setAlignment(smudgeButton, Qt::AlignLeft);

	drawLay->addWidget(penButton,3,0); drawLay->setAlignment(penButton, Qt::AlignRight);
	drawLay->addWidget(handButton,3,1); drawLay->setAlignment(handButton, Qt::AlignLeft);

	drawLay->addWidget(pencilButton,4,0); drawLay->setAlignment(pencilButton, Qt::AlignRight);
	drawLay->addWidget(bucketButton,4,1); drawLay->setAlignment(bucketButton, Qt::AlignLeft);


	drawLay->addWidget(eyedropperButton,5,0); drawLay->setAlignment(eyedropperButton, Qt::AlignRight);
	drawLay->addWidget(eraserButton,5,1); drawLay->setAlignment(eraserButton, Qt::AlignLeft);
	//drawLay->addWidget(mirrorButton,5,1); drawLay->setAlignment(mirrorButton, Qt::AlignLeft);

	optionLay->setMargin(8);
	optionLay->setSpacing(8);
	optionLay->addWidget(colourLabel,6,0);
	optionLay->addWidget(choseColour,6,1);
 //optionLay->addWidget(thinLinesButton,6,2);

	//optionLay->addWidget(sizeSlider,7,0,1,2);


	/*QGroupBox* sizeBox = new QGroupBox("Size");
	QHBoxLayout* sizeBoxLay = new QHBoxLayout();
	sizeBoxLay->addWidget(sizeSlider2);
	sizeBoxLay->setMargin(0);
	sizeBoxLay->setSpacing(0);
	sizeBox->setLayout(sizeBoxLay);*/

	optionLay->addWidget(sizeSlider,8,0,1,2);
	optionLay->addWidget(featherSlider,9,0,1,2);
	//optionLay->addWidget(opacitySlider,10,0,1,2);

	optionLay->addWidget(usePressureBox,11,0,1,2);
	optionLay->addWidget(preserveAlphaBox,12,0,1,2);
	optionLay->addWidget(followContourBox,13,0,1,2);
	optionLay->addWidget(makeInvisibleBox,14,0,1,2);
        optionLay->setRowStretch(15,1);

	displayLay->setMargin(4);
	displayLay->setSpacing(0);
	displayLay->addWidget(mirrorButton,0,0);
	displayLay->addWidget(thinLinesButton,0,1);
	displayLay->addWidget(outlinesButton,1,1);
	displayLay->addWidget(mirrorButtonV,1,0);
	displayLay->addWidget(onionPrev,0,2);
	displayLay->addWidget(onionNext,1,2);

	keyLay->setMargin(4);
	keyLay->setSpacing(0);
	keyLay->addWidget(add,0,0);
	keyLay->addWidget(rm,0,1);

	onionLay->setMargin(4);
	onionLay->setSpacing(0);
	// -- mj
	//onionLay->addWidget(onionPrevBox, 0,0);
	//onionLay->addWidget(onionNextBox, 1,0);

	timeLay->setMargin(4);
	timeLay->setSpacing(0);
	timeLay->addWidget(play,0,0);
	timeLay->addWidget(fpsBox,1,0);
	timeLay->addWidget(loopBox,2,0,1,-1);
	timeLay->addWidget(soundBox,3,0,1,-1);

	drawGroup->setLayout(drawLay);
	optionGroup->setLayout(optionLay);
	displayGroup->setLayout(displayLay);
	keyGroup->setLayout(keyLay);
	onionGroup->setLayout(onionLay);
	timeGroup->setLayout(timeLay);

	//keyGroup->setFlat(true);
	//onionGroup->setFlat(true);
	//timeGroup->setFlat(true);

	//drawGroup->setMaximumSize( QSize(3*32,6*32+1) );
	drawGroup->setMaximumHeight(6*32+1);

	drawPalette->setMaximumHeight(200);
	//drawPalette->setFeatures(drawPalette->features() | QDockWidget::DockWidgetVerticalTitleBar);
        //optionPalette->setMaximumHeight(300);
        //optionGroup->setMaximumHeight(160);
	displayPalette->setMaximumHeight(60);
	keyGroup->setMaximumHeight(60);
	onionGroup->setMaximumHeight(60);

	/*butlay->setMargin(0);
	butlay->setSpacing(2);
    butlay->addWidget(drawGroup);
		butlay->addWidget(optionGroup);
    butlay->addWidget(keyGroup);
    butlay->addWidget(onionGroup);
    butlay->addWidget(timeGroup);

    framelay->addLayout(butlay);
    //framelay->addWidget(framecounter);

    setLayout(framelay);*/

	connect(add, SIGNAL(clicked()), this, SIGNAL(addClick()));
	connect(rm, SIGNAL(clicked()), this, SIGNAL(rmClick()));

	connect(pencilButton, SIGNAL(clicked()), this, SIGNAL(pencilClick()));
	connect(selectButton, SIGNAL(clicked()), this, SIGNAL(selectClick()));
	connect(moveButton, SIGNAL(clicked()), this, SIGNAL(moveClick()));
	connect(handButton, SIGNAL(clicked()), this, SIGNAL(handClick()));
	connect(eraserButton, SIGNAL(clicked()), this, SIGNAL(eraserClick()));
	connect(penButton, SIGNAL(clicked()), this, SIGNAL(penClick()));
	connect(polylineButton, SIGNAL(clicked()), this, SIGNAL(polylineClick()));
	connect(bucketButton, SIGNAL(clicked()), this, SIGNAL(bucketClick()));
	connect(eyedropperButton, SIGNAL(clicked()), this, SIGNAL(eyedropperClick()));
	connect(colouringButton, SIGNAL(clicked()), this, SIGNAL(colouringClick()));
	connect(smudgeButton, SIGNAL(clicked()), this, SIGNAL(smudgeClick()));

	connect(thinLinesButton, SIGNAL(clicked()), this, SIGNAL(thinLinesClick()));
	connect(outlinesButton, SIGNAL(clicked()), this, SIGNAL(outlinesClick()));
	//connect(outlinesButton, SIGNAL(pressed()), this, SIGNAL(outlinesPressed()));
	//connect(outlinesButton, SIGNAL(released()), this, SIGNAL(outlinesReleased()));

	connect(usePressureBox, SIGNAL(clicked(bool)), this, SLOT(pressureClick(bool)));
	connect(makeInvisibleBox, SIGNAL(clicked(bool)), this, SLOT(invisibleClick(bool)));
	connect(preserveAlphaBox, SIGNAL(clicked(bool)), this, SLOT(preserveAlphaClick(bool)));
	connect(followContourBox, SIGNAL(clicked(bool)), this, SLOT(followContourClick(bool)));
	connect(sizeSlider, SIGNAL(valueChanged(qreal)), this, SIGNAL(widthClick(qreal)));
	connect(featherSlider, SIGNAL(valueChanged(qreal)), this, SIGNAL(featherClick(qreal)));
	connect(opacitySlider, SIGNAL(valueChanged(qreal)), this, SIGNAL(opacityClick(qreal)));

	// -- mj
		connect(onionPrev, SIGNAL(clicked(bool)), this, SIGNAL(togglePrev(bool)));
		connect(onionNext, SIGNAL(clicked(bool)), this, SIGNAL(toggleNext(bool)));


	connect(choseColour, SIGNAL(clicked()), this, SIGNAL(colourClick()));
	connect(clearButton, SIGNAL(clicked()), this, SIGNAL(clearClick()));
	connect(mirrorButton, SIGNAL(clicked()), this, SIGNAL(mirrorClick()));
	connect(mirrorButtonV, SIGNAL(clicked()), this, SIGNAL(mirrorVClick()));

	connect(play, SIGNAL(clicked()), this, SIGNAL(playClick()));
	connect(loopBox, SIGNAL(stateChanged(int)), this, SIGNAL(loopClick()));
	connect(soundBox, SIGNAL(stateChanged(int)), this, SIGNAL(soundClick()));

	connect(fpsBox,SIGNAL(valueChanged(int)), this, SIGNAL(fpsClick(int)));

	connect(pencilButton, SIGNAL(clicked()), this, SLOT(changePencilButton()));
	connect(selectButton, SIGNAL(clicked()), this, SLOT(changeSelectButton()));
	connect(moveButton, SIGNAL(clicked()), this, SLOT(changeMoveButton()));
	connect(handButton, SIGNAL(clicked()), this, SLOT(changeHandButton()));
	connect(eraserButton, SIGNAL(clicked()), this, SLOT(changeEraserButton()));
	connect(penButton, SIGNAL(clicked()), this, SLOT(changePenButton()));
	connect(polylineButton, SIGNAL(clicked()), this, SLOT(changePolylineButton()));
	connect(bucketButton, SIGNAL(clicked()), this, SLOT(changeBucketButton()));
	connect(eyedropperButton, SIGNAL(clicked()), this, SLOT(changeEyedropperButton()));
	connect(colouringButton, SIGNAL(clicked()), this, SLOT(changeColouringButton()));
	connect(smudgeButton, SIGNAL(clicked()), this, SLOT(changeSmudgeButton()));

	//connect(thinLinesButton, SIGNAL(clicked()), this, SLOT(changeThinLinesButton()));
}

void ToolSet::newToolButton(QToolButton* &toolButton) {
	toolButton = new QToolButton(this);
	toolButton->setAutoRaise(true);
	toolButton->setIconSize( QSize(24,24) );
	toolButton->setFixedSize(32,32);
}

void ToolSet::setCounter(int x) {
	//framecounter->setText(QString::number(x));
}

void ToolSet::setWidth(qreal x) {
	if(x < 0) {
		sizeSlider->setEnabled(false);
	} else {
		sizeSlider->setEnabled(true);
		sizeSlider->setValue(x);
	}
}

void ToolSet::setFeather(qreal x) {
	if(x < 0) {
		featherSlider->setEnabled(false);
	} else {
		featherSlider->setEnabled(true);
		featherSlider->setValue(x);
	}
}

void ToolSet::setOpacity(qreal x) {
	if(x < 0) {
		opacitySlider->setEnabled(false);
	} else {
		opacitySlider->setEnabled(true);
		opacitySlider->setValue(x);
	}
}

void ToolSet::setPressure(int x) { // x = -1, 0, 1
	if(x<0) {
		usePressureBox->setEnabled(false);
	} else {
		usePressureBox->setEnabled(true);
		usePressureBox->setChecked(x>0);
	}
}

void ToolSet::pressureClick(bool x) {
	int y = 0;
	if(x) y = 1;
	emit pressureClick(y);
}

void ToolSet::setInvisibility(int x) { // x = -1, 0, 1
	if(x<0) {
		makeInvisibleBox->setEnabled(false);
	} else {
		makeInvisibleBox->setEnabled(true);
		makeInvisibleBox->setChecked(x>0);
	}
}

void ToolSet::invisibleClick(bool x) {
	int y = 0;
	if(x) y = 1;
	emit invisibleClick(y);
}

void ToolSet::setPreserveAlpha(int x) { // x = -1, 0, 1
	if(x<0) {
		preserveAlphaBox->setEnabled(false);
	} else {
		preserveAlphaBox->setEnabled(true);
		preserveAlphaBox->setChecked(x>0);
	}
}

void ToolSet::preserveAlphaClick(bool x) {
	int y = 0;
	if(x) y = 1;
	emit preserveAlphaClick(y);
}

void ToolSet::setFollowContour(int x) { // x = -1, 0, 1
	if(x<0) {
		followContourBox->setEnabled(false);
	} else {
		followContourBox->setEnabled(true);
		followContourBox->setChecked(x>0);
	}
}

void ToolSet::followContourClick(bool x) {
	int y = 0;
	if(x) y = 1;
	emit followContourClick(y);
}

void ToolSet::setColour(QColor x) {
	QPixmap colourSwatch(30,30);
	colourSwatch.fill(x);
	choseColour->setIcon(colourSwatch);
}

void ToolSet::changePencilButton() {
	deselectAllTools();
	pencilButton->setChecked(true);
}

void ToolSet::changeEraserButton() {
	deselectAllTools();
	eraserButton->setChecked(true);
}

void ToolSet::changeSelectButton() {
	deselectAllTools();
	selectButton->setChecked(true);
}

void ToolSet::changeMoveButton() {
	deselectAllTools();
	moveButton->setChecked(true);
}

void ToolSet::changeHandButton() {
	deselectAllTools();
	handButton->setChecked(true);
}

void ToolSet::changePenButton() {
	deselectAllTools();
	penButton->setChecked(true);
}

void ToolSet::changePolylineButton() {
	deselectAllTools();
	polylineButton->setChecked(true);
}

void ToolSet::changeBucketButton() {
	deselectAllTools();
	bucketButton->setChecked(true);
}

void ToolSet::changeEyedropperButton() {
	deselectAllTools();
	eyedropperButton->setChecked(true);
}

void ToolSet::changeColouringButton() {
	deselectAllTools();
	colouringButton->setChecked(true);
}

void ToolSet::changeSmudgeButton() {
	deselectAllTools();
	smudgeButton->setChecked(true);
}

void ToolSet::changeOutlinesButton(bool trueOrFalse) {
	outlinesButton->setChecked(trueOrFalse);
}


void ToolSet::changeThinLinesButton(bool trueOrFalse) {
	thinLinesButton->setChecked(trueOrFalse);
}


void ToolSet::resetMirror() {
	mirrorButton->setChecked(false);
}

void ToolSet::resetMirrorV() {
	mirrorButtonV->setChecked(false);
}

void ToolSet::deselectAllTools() {
	pencilButton->setChecked(false);
	eraserButton->setChecked(false);
	selectButton->setChecked(false);
	moveButton->setChecked(false);
	handButton->setChecked(false);
	penButton->setChecked(false);
	polylineButton->setChecked(false);
	bucketButton->setChecked(false);
	eyedropperButton->setChecked(false);
	colouringButton->setChecked(false);
	smudgeButton->setChecked(false);
}

void ToolSet::onionPrevChanged(bool checked)
{
    onionPrev->setChecked(checked);
}

void ToolSet::onionNextChanged(bool checked)
{
    onionNext->setChecked(checked);
}