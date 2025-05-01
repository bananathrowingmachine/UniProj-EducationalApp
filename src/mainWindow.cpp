/**
 * The main view window of the program. Maintains the model object, connecting it with user inputs, as well as organizing everything related in inputs and outputs.
 * Contains a lot of sound systems, images, collision detection, signals, slots, and connect methods that link everything together.
 *
 * 
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 *
 * Checked by [redacted] and [redacted]
 * April 22, 2025
 **/

#include "mainWindow.h"
#include "ui_mainWindow.h"
#include "ingredientcontactlistener.h"
#include <Box2D/Box2D.h>
#include <QFontDatabase>
#include <QPainter>
#include <QVBoxLayout>
#include <QSoundEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , world(b2Vec2(0.0f, -10.0f))
    , physicsTimer(new QTimer(this))
{
    ui->setupUi(this);

    // introduction.setModal(true);
    // introduction.show();

    // Adds the drinks to the drop down menu.
    ui->recipeList->addItems(QStringList() << "" << "Margarita" << "Martini" << "Pina Colada" << "Manhattan" << "Blue Blazer");

    //Setting up the text for the recipe.
    ui->recipeText->hide();
    ui->recipeText->setAlignment(Qt::AlignCenter);
    ui->recipeText->setWordWrap(true);
    ui->recipeText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //Adding a chalk font to the project
    int id=QFontDatabase::addApplicationFont(":/font/Chalk-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont chalk(family);
    ui->recipeText->setFont(chalk);
    ui->next->hide();
    ui->back->hide();

    // Setup contact listener for collision detection
    IngredientContactListener* contactListener = new IngredientContactListener();
    world.SetContactListener(contactListener);

    //Allows the shelf to be scrollable.
    QVBoxLayout* verticalLayout = new QVBoxLayout(ui->scrollAreas);
    for (int i = 0; i < 40; ++i) {
        QHBoxLayout* hLayout = new QHBoxLayout();
        for (int j = 0; j < 5; ++j) {
            QLabel* label = new QLabel("", this);
            hLayout->addWidget(label);
        }
        verticalLayout->addLayout(hLayout);
    }

    // Drink buttons
    connect(ui->tequila, &QPushButton::clicked, this, &MainWindow::ingredientAppears);
    connect(ui->tripleSec, &QPushButton::clicked, this, &MainWindow::ingredientAppears);
    connect(ui->limeJuice, &QPushButton::clicked, this, &MainWindow::ingredientAppears);
    connect(ui->agaveSyrup, &QPushButton::clicked, this, &MainWindow::ingredientAppears);
    connect(ui->bitters, &QPushButton::clicked, this, &MainWindow::ingredientAppears);
    connect(ui->blueCuracao, &QPushButton::clicked, this, &MainWindow::ingredientAppears);
    connect(ui->coconutCream, &QPushButton::clicked, this, &MainWindow::ingredientAppears);
    connect(ui->gin, &QPushButton::clicked, this, &MainWindow::ingredientAppears);
    connect(ui->pineappleJuice, &QPushButton::clicked, this, &MainWindow::ingredientAppears);
    connect(ui->sweetVermouth, &QPushButton::clicked, this, &MainWindow::ingredientAppears);
    connect(ui->vermouth, &QPushButton::clicked, this, &MainWindow::ingredientAppears);
    connect(ui->whiskey, &QPushButton::clicked, this, &MainWindow::ingredientAppears);
    connect(ui->whiteRum, &QPushButton::clicked, this, &MainWindow::ingredientAppears);


    // Garnish buttons
    connect(ui->saltRim, &QPushButton::clicked, this, &MainWindow::ingredientAppears);
    connect(ui->lime, &QPushButton::clicked, this, &MainWindow::ingredientAppears);
    connect(ui->ice, &QPushButton::clicked, this, &MainWindow::ingredientAppears);
    connect(ui->olive, &QPushButton::clicked, this, &MainWindow::ingredientAppears);
    connect(ui->cherry, &QPushButton::clicked, this, &MainWindow::ingredientAppears);
    connect(ui->crushedIce, &QPushButton::clicked, this, &MainWindow::ingredientAppears);


    // Reset, restart, and submit buttons
    connect(ui->ResetPhysicsButton, &QPushButton::clicked, this, &MainWindow::resetPositions);
    connect(ui->restartGameButton, &QPushButton::clicked, this, &MainWindow::restartGame);
    connect(ui->submitDrink, &QPushButton::clicked, &modelClass , &model::submitDrink);

    //Forward and back for the recipe text.
    connect(ui->next, &QPushButton::clicked, this, &MainWindow::onNextClicked);
    connect(ui->back, &QPushButton::clicked, this, &MainWindow::onBackClicked);

    //Handling the score dialog window.
    connect(&modelClass, &model::sendingUserScore, this, &MainWindow::displayUserScore);
    connect(this, &MainWindow::sendScoreToDialog, &scoreWindow, &ScoreWindow::showScore);
    connect(&scoreWindow, &ScoreWindow::sendingRestartGame, this, &MainWindow::restartGame);

    //Handling the measuring of the liquid.
    connect(&measure, &MeasureDialog::windowClosed, this, &MainWindow::measureDialogClosed);
    connect(&measure, &MeasureDialog::sendInput, this, &MainWindow::ingredientMeasuredWithJigger); // For updating Jigger view
    connect(&measure, &MeasureDialog::sendInput, &jiggerModel, &JiggerTool::setContents); // For updating Jigger model

    //Handling the contact of ingredient objects.
    connect(contactListener, &IngredientContactListener::mixerJiggerCollision, this, &MainWindow::pourJiggerIntoMixer); // Mixer-Jigger collision -> MainWindow
    connect(contactListener, &IngredientContactListener::measureOpened, this, &MainWindow::measureDialogOpened);
    connect(contactListener, &IngredientContactListener::measureOpened, &measure, &MeasureDialog::measureDialogOpened);
    connect(contactListener, &IngredientContactListener::collisionOccured, this, &MainWindow::playCollisionSound);
    connect(this, &MainWindow::pourMixerIntoGlass, this, &MainWindow::pouredMixerIntoGlass);

    //Adds user's ingredient to the drink.
    connect(this, &MainWindow::addIngredientToDrinkModel, &modelClass, &model::addIngredient);

    //Shaking the contents in the mixer.
    connect(this, &MainWindow::shakingMixer, &modelClass, &model::shakeIngredient);

    // Sending recipe and showing recipe.
    connect(this, &MainWindow::showSelectedDrink, &modelClass, &model::startRound);
    connect(&modelClass, &model::sendRecipe, this, &MainWindow::displayRecipe);

    // The drop down recipe list
    connect(ui->recipeList, &QComboBox::currentTextChanged, this, &MainWindow::recipeChosen);

    //Tells main window to add a garnish to the box above the cup.
    connect(contactListener, &IngredientContactListener::addGarnishToGlass, this, &MainWindow::addGarnishToLayout);

    //Tells the game to pour from mixer to glass.
    connect(&pourDialog, &PourWindow::pourDrink, this, &MainWindow::pouringDrink);
    connect(contactListener, &IngredientContactListener::callPouringDialog, this, &MainWindow::openPouringDialog);
    connect(this, &MainWindow::sendStringsToPourDialog, &pourDialog, &PourWindow::getStringsFromView);
    connect(&pourDialog, &PourWindow::windowClosed, this, &MainWindow::pourWindowClosed);

    //Handles the visability of the recipe text.
    ui->recipeCheckBox->hide();
    connect(ui->recipeCheckBox, &QCheckBox::checkStateChanged, this, &MainWindow::recipeCheckStateChanged);

    //Help for the user.
    connect(ui->helpButton, &QPushButton::clicked, this, &MainWindow::showHelpDialog);

    //Disabling the restart and reset buttons to start.
    ui->submitDrink->setEnabled(false);
    ui->restartGameButton->setEnabled(false);
    ui->submitDrink->setStyleSheet("background-color: gray;");
    ui->restartGameButton->setStyleSheet("background-color: gray;");

    // Setup timer for physics updates
    physicsTimer = new QTimer(this);
    physicsTimer->setInterval(10);
    connect(physicsTimer, &QTimer::timeout, this, &MainWindow::updateWorld);
    physicsTimer->start();

    float scale = 100.0f;

    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(-5.0f, -9.9f + (ui->table->height() / scale)); //-10.0f + (ui->table->pos().y() / scale)
    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    groundBody = world.CreateBody(&groundBodyDef);
    groundBody->SetUserData(this);
    // Define the ground box shape.
    b2PolygonShape groundBox;
    // The extents are the half-widths of the box.
    groundBox.SetAsBox(50.0f, 10.0f);
    // Add the ground fixture to the ground body.
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Define the left wall body.
    b2BodyDef leftWallBodyDef;
    leftWallBodyDef.position.Set(-10.0f, 0.0f);
    leftWallBody = world.CreateBody(&leftWallBodyDef);
    b2PolygonShape leftWallBox;
    leftWallBox.SetAsBox(10.0f, 50.0f); // The extents are the half-widths of the box.
    leftWallBody->CreateFixture(&leftWallBox, 0.0f); // Add the ground fixture to the ground body.

    // Define the right wall body.
    b2BodyDef rightWallBodyDef;
    rightWallBodyDef.position.Set(10.0 + width()/scale, 0.0f);
    rightWallBody = world.CreateBody(&rightWallBodyDef);
    b2PolygonShape rightWallBox;
    rightWallBox.SetAsBox(10.0f, 50.0f); // The extents are the half-widths of the box.
    rightWallBody->CreateFixture(&rightWallBox, 0.0f); // Add the ground fixture to the ground body.

    // Create the mixer, jigger, and glass objects
    mixerObj = new PhysicsObject(&world, QImage(":/icons/drinkShaker.png"), this, 0.4, 1.0);
    mixerObj->setPosition(2.5f, 2.0f);
    mixerObj->setName("Mixer");
    mixerObj->show();

    jiggerObj = new PhysicsObject(&world, QImage(":/icons/jigger.png"), this, 0.2, 0.5);
    jiggerObj->setPosition(3.5f, 2.0f);
    jiggerObj->setName("Jigger");
    jiggerObj->show();

    glassObj = new PhysicsObject(&world, QImage(":/icons/martiniGlass.png"), this, 0.4, 0.6, true);
    glassObj->setPosition(1.0f, 1.9f);
    glassObj->setName("Glass");
    glassObj->show();

    // Creates the more basic sounds (collisions require a more advanced system), and their rate limiters.
    shakeSFX = new QSoundEffect(this);
    shakeSFX->setSource(QUrl("qrc:/SFX/ShakerSFX.wav"));
    shakeSFX->setVolume(1.0f);
    shakeSFX->setLoopCount(QSoundEffect::Infinite);
    pourSFX = new QSoundEffect(this);
    pourSFX->setSource(QUrl("qrc:/SFX/PourSFX.wav"));

    collisionSoundRateLimiter.start();
    shakeSoundRateLimiter.start();

    consecutiveShakes = 0;
}

MainWindow::~MainWindow()
{
    qDeleteAll(ingredientsInWorld);
    ingredientsInWorld.clear();
    qDeleteAll(drinkTools);
    drinkTools.clear();
    qDeleteAll(activeSounds);
    activeSounds.clear();
    clearLayout(ui->addedGarnishes);
    delete ui;
    delete physicsTimer;
    delete mixerObj;
    delete jiggerObj;
    delete glassObj;
    delete shakeSFX;
    delete pourSFX;
}

void MainWindow::updateWorld(){
    // Clean up marked objects first
    for (int i = ingredientsInWorld.size() - 1; i >= 0; i--) {
        if(ingredientsInWorld[i]->isMarkedForDeletion()){
            PhysicsObject* obj = ingredientsInWorld.takeAt(i);
            // Check if we're currently dragging this object
            if(mouseJoint && mouseJoint->GetBodyB() == obj->body){
                destroyMouseJoint();
            }
            world.DestroyBody(obj->body);
            delete obj;
        }
    }

    float32 timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
    world.Step(timeStep, velocityIterations, positionIterations);

    // Check if user is shaking the mixer
    if(mixerObj->body->GetLinearVelocity().LengthSquared() > 1000.0f){
        if(!shakeSFX->isPlaying()) {
            shakeSoundRateLimiter.restart();
            shakeSFX->play();
        }
        // Every 20 ms, consecutive shakes will increment if shaking, every 100 ms itll decrement by 3 until it hits 0
        // Therefore in the span of 1 second you can (theoretically) gain 50 shake points, and lose 30. Need 15 to count as shaken.
        else if(shakeSoundRateLimiter.hasExpired(20)) {
            shakeSoundRateLimiter.restart();
            consecutiveShakes++;
        }

        if(!mixerIsShaken && consecutiveShakes > 15 && mixerTooltipElements.size() > 0) {
            consecutiveShakes = 0;
            shakeMixerView();
            emit shakingMixer();
        }
    }
    else if (shakeSoundRateLimiter.hasExpired(100) && (consecutiveShakes > 0 || shakeSFX->isPlaying())) {
        if (consecutiveShakes > 0)
            consecutiveShakes = std::max(consecutiveShakes - 3, 0);
        if (shakeSFX->isPlaying())
            shakeSFX->stop();
        shakeSoundRateLimiter.restart();
    }

    // Update any bottle's physics
    for(PhysicsObject* physObj : std::as_const(ingredientsInWorld)){
        physObj->updateObject(height());
    }
    // Update jigger, mixer, and glass
    jiggerObj->updateObject(height());
    mixerObj->updateObject(height());
    glassObj->updateObject(height());

    update();
}

// Pouring/Mixing Methods
void MainWindow::pouringDrink(QString object1, QString object2){

    // Check what is being poured into what
    if((object1 == "Glass" || object2 == "Glass") && (object1 == "Mixer" || object2 == "Mixer"))
        emit pourMixerIntoGlass();
    else if((object1 == "Mixer" || object2 == "Mixer") && (object1 == "Bitters" || object2 == "Bitters"))
        pourDirectlyIntoMixer("bitters", "Dash of bitters");
    else if((object1 == "Mixer" || object2 == "Mixer") && (object1 == "Ice" || object2 == "Ice"))
        pourDirectlyIntoMixer("ice", "Filled with ice");

    pourDialog.close();
}

void MainWindow::pouredMixerIntoGlass(){
    ui->submitDrink->setEnabled(true);
    ui->submitDrink->setStyleSheet("background-color: green;");

    // Reset mixer image.
    mixerObj->setImage(QImage(":/icons/drinkShaker.png"));
    mixerIsShaken = false;

    // Set glass image color to show it contains something
    glassObj->setImage(QImage(":/icons/martiniGlass_FULL.png"));
    glassObj->setToolTip(mixerObj->toolTip());
    mixerTooltipElements.clear();
    updateMixerTooltip();
    pourSFX->play();
}

void MainWindow::pourJiggerIntoMixer(){
    if(!jiggerModel.isEmpty()){
        float jiggerAmount = jiggerModel.getAmount();
        QString jiggerIngredientName = jiggerModel.getIngredientName();
        emit addIngredientToDrinkModel(jiggerIngredientName, jiggerAmount, false);
        jiggerModel.clearContents();

        jiggerObj->setToolTip("Empty");
        jiggerObj->setImage(QImage(":/icons/jigger.png"));

        // Update mixer tooltip
        mixerTooltipElements.push_back(QString::number(jiggerAmount) + "oz. of " + jiggerIngredientName);
        updateMixerTooltip();
        pourSFX->play();
    }
}

void MainWindow::pourDirectlyIntoMixer(QString name, QString tooltip){
    if(!mixerTooltipElements.contains(tooltip)) {
        emit addIngredientToDrinkModel(name, 0, false);
        mixerTooltipElements.push_back(tooltip);
        updateMixerTooltip();
    }
}

void MainWindow::ingredientMeasuredWithJigger(float amount, const QString& ingredientName){
    destroyMouseJoint();
    clearPhysicsObjects();

    jiggerObj->setToolTip("Contains " + QString::number(amount) + "oz. of " + ingredientName);
    jiggerObj->setImage(QImage(":/icons/jigger_FULL.png"));

    pourSFX->play();
}

void MainWindow::updateMixerTooltip(){
    QString tooltip = "Contains: <br>";
    for(const QString &element : std::as_const(mixerTooltipElements)){
        tooltip += element + "<br>";
    }
    mixerObj->setToolTip(tooltip);
}

void MainWindow::shakeMixerView(){
    mixerObj->setImage(QImage(":/icons/drinkShaker_SHAKEN.png"));
    mixerTooltipElements.push_back("Shaken");
    updateMixerTooltip();
    mixerIsShaken = true;
}

// Restart/Reset Methods
void MainWindow::restartGame(){
    //hide elements
    scoreWindow.hide();
    ui->recipeText->hide();
    ui->recipeCheckBox->hide();

    //show elements
    ui->selectDrink->show();
    ui->recipeList->show();

    //clear elements
    clearLayout(ui->addedGarnishes);
    clearPhysicsObjects();
    mixerTooltipElements.clear();
    mixerIsShaken = false;
    mixerObj->setToolTip("Empty");
    glassObj->setToolTip("Empty");
    jiggerObj->setToolTip("Empty");
    ui->recipeList->clear();
    glassObj->setImage(QImage(":/icons/martiniGlass.png"));
    jiggerObj->setImage(QImage(":/icons/jigger.png"));

    //restart the model without a recipe being sent.
    emit showSelectedDrink("Blank");

    // Restart the dropdown menu
    ui->recipeList->addItems(QStringList() << "" << "Margarita" << "Martini" << "Pina Colada" << "Manhattan" << "Blue Blazer");

    //Disable the submit and restart buttons.
    ui->submitDrink->setEnabled(false);
    ui->submitDrink->setStyleSheet("background-color: gray;");
    ui->restartGameButton->setEnabled(false);
    ui->restartGameButton->setStyleSheet("background-color: gray;");

    resetPositions();
}

void MainWindow::resetPositions(){
    // Reset position
    jiggerObj->body->SetTransform(b2Vec2(3.5f, 1.75f), 0);
    mixerObj->body->SetTransform(b2Vec2(2.5f, 2.25f), 0);
    glassObj->body->SetTransform(b2Vec2(1.0f, 1.9f), 0);

    // Reset linear and angular velocity to 0
    jiggerObj->body->SetLinearVelocity(b2Vec2(0,0));
    mixerObj->body->SetLinearVelocity(b2Vec2(0,0));
    glassObj->body->SetLinearVelocity(b2Vec2(0,0));
    jiggerObj->body->SetAngularVelocity(0);
    mixerObj->body->SetAngularVelocity(0);
    glassObj->body->SetAngularVelocity(0);
}

void MainWindow::clearLayout(QLayout* layout) {
    if (!layout)
        return;

    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        // If the item is a layout, recursively clear it
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        // If the item is a widget, delete the widget
        else if (item->widget()) {
            delete item->widget();
        }
        // Delete the item itself
        delete item;
    }
}

void MainWindow::clearPhysicsObjects(){
    // Mark all objects for deletion in the IngredientsInWorld vector
    for (PhysicsObject* physObj : ingredientsInWorld) {
        physObj->markForDeletion();
    }
}

// Recipe Methods
void MainWindow::recipeChosen(){
    QString selectedDrink = ui->recipeList->currentText();
    //Tell the model which recipe we want, restart the gui, and get the recipe from the model.
    emit showSelectedDrink(selectedDrink);
    restartGame();
    emit showSelectedDrink(selectedDrink);
    ui->restartGameButton->setEnabled(true);
    ui->restartGameButton->setStyleSheet("background-color: red;");
}

void MainWindow::displayRecipe(QStringList recipe){
    ui->recipeCheckBox->show();
    instructionSteps=recipe;
    ui->selectDrink->hide();
    ui->recipeList->hide();
    ui->recipeText->show();
    currentStepIndex=0;
    ui->recipeText->setText(recipe[currentStepIndex]);
    ui->next->show();
    ui->back->show();
    ui->next->setEnabled(true);
    ui->back->setEnabled(false);
}

// Button Methods
void MainWindow::onNextClicked() {
    if (currentStepIndex < instructionSteps.size() - 1) {
        currentStepIndex++;
        ui->recipeText->setText(instructionSteps[currentStepIndex]);
    }
    ui->next->setEnabled(currentStepIndex < instructionSteps.size() - 1);
    ui->back->setEnabled(currentStepIndex > 0);
}

void MainWindow::onBackClicked(){

    if (currentStepIndex >0) {
        currentStepIndex--;
        ui->recipeText->setText(instructionSteps[currentStepIndex]);
    }
    ui->next->setEnabled(currentStepIndex < instructionSteps.size() - 1);
    ui->back->setEnabled(currentStepIndex > 0);
}

// Dialog Methods
void MainWindow::measureDialogOpened(){
    measure.setModal(true);
    measure.show();
}

void MainWindow::measureDialogClosed(){
    destroyMouseJoint();
    clearPhysicsObjects();

    if(jiggerModel.isEmpty())
        jiggerObj->setImage(QImage(":/icons/jigger.png"));
}

void MainWindow::openPouringDialog(QString object1, QString object2){
    emit sendStringsToPourDialog(object1, object2);
    pourDialog.setModal(true);
    pourDialog.show();
}

void MainWindow::pourWindowClosed(){
    destroyMouseJoint();
    clearPhysicsObjects();
}

void MainWindow::displayUserScore(int score, QString explination){
    scoreWindow.show();
    emit sendScoreToDialog(score, explination);
}

// Mouse Methods
void MainWindow::mousePressEvent(QMouseEvent *event){
    QPoint pos = event->pos();
    float scale = 100.0f;

    b2Vec2 worldPoint = b2Vec2(pos.x() / scale, (height() - pos.y()) / scale);

    SelectedBodyCallback callback(worldPoint);
    b2AABB aabb;
    aabb.lowerBound = worldPoint - b2Vec2(0.001f, 0.001f);
    aabb.upperBound = worldPoint + b2Vec2(0.001f, 0.001f);
    world.QueryAABB(&callback, aabb);

    if (callback.body && callback.body->GetType() == b2_dynamicBody) {
        b2MouseJointDef md;
        md.bodyA = groundBody;
        md.bodyB = callback.body;
        md.target = worldPoint;
        md.maxForce = 10000.0f * callback.body->GetMass();
        md.dampingRatio = 1.0f;
        md.frequencyHz = 30.0f;
        mouseJoint = (b2MouseJoint*)world.CreateJoint(&md);
        callback.body->SetAwake(true);
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    if (mouseJoint) {
        float scale = 100.0f;
        QPoint pos = event->pos();
        b2Vec2 worldPoint = b2Vec2(pos.x() / scale, (height() - pos.y()) / scale);
        mouseJoint->SetTarget(worldPoint);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    destroyMouseJoint();
}

void MainWindow::destroyMouseJoint(){
    if (mouseJoint) {
        if (mouseJoint->GetBodyA() && mouseJoint->GetBodyB()) {
            world.DestroyJoint(mouseJoint);
        }
        mouseJoint = nullptr;
    }
}

// SFX Method
void MainWindow::playCollisionSound(){
    if (collisionSoundRateLimiter.hasExpired(500)) {
        QSoundEffect* collisionSFX = new QSoundEffect(this);
        collisionSFX->setSource(QUrl("qrc:/SFX/metal pipe falling sound effect.wav"));
        collisionSFX->setVolume(0.25f);
        connect(collisionSFX, &QSoundEffect::playingChanged, this, [this, collisionSFX]() {
            if (!collisionSFX->isPlaying()) {
                activeSounds.removeOne(collisionSFX);
                collisionSFX->deleteLater();
            }
        });
        activeSounds.append(collisionSFX);
        collisionSFX->play();
        collisionSoundRateLimiter.restart();
    }
}

// Ingredient Methods
void MainWindow::addGarnishToLayout(QString garnish){
    if(garnish.isEmpty()){
        return;
    }

    QLabel* imageLabel = new QLabel(this); // Deleted along with mainWindow(this).

    if (garnish == "lime"){
        QPixmap pixmap(":/icons/limeSlice.png");
        QPixmap scaledPixmap = pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(scaledPixmap);
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        ui->addedGarnishes->addWidget(imageLabel);
    }
    else if (garnish == "cherry"){
        QPixmap pixmap(":/icons/maraschinoCherry.png");
        QPixmap scaledPixmap = pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(scaledPixmap);
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        ui->addedGarnishes->addWidget(imageLabel);
    }
    else if (garnish == "ice"){
        QPixmap pixmap(":/icons/iceCubes.png");
        QPixmap scaledPixmap = pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(scaledPixmap);
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        ui->addedGarnishes->addWidget(imageLabel);
    }
    else if (garnish == "saltRim"){
        QPixmap pixmap(":/icons/saltRim.png");
        QPixmap scaledPixmap = pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(scaledPixmap);
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        ui->addedGarnishes->addWidget(imageLabel);
    }
    else if (garnish == "olive"){
        QPixmap pixmap(":/icons/olives.png");
        QPixmap scaledPixmap = pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(scaledPixmap);
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        ui->addedGarnishes->addWidget(imageLabel);
    }
    else if (garnish == "crushedIce"){
        QPixmap pixmap(":/icons/crushedIce.jpg");
        QPixmap scaledPixmap = pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(scaledPixmap);
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        ui->addedGarnishes->addWidget(imageLabel);
    }

    emit addIngredientToDrinkModel(garnish, 0, true);
}

void MainWindow::ingredientAppears(){
    QPushButton* button = qobject_cast<QPushButton*>(sender());  // Get the sender of the signal
    if (button) {
        QString buttonName = button->objectName();

        if (buttonName == "tequila"){
            clearPhysicsObjects();
            PhysicsObject* tequila = new PhysicsObject(&world, QImage(":/icons/tequila.png"), this, 0.28, 1.0);
            tequila->setName(buttonName);
            ingredientsInWorld.push_back(tequila);
            tequila->show();
        }
        else if (buttonName == "tripleSec"){
            clearPhysicsObjects();
            PhysicsObject* tripleSec = new PhysicsObject(&world, QImage(":/icons/tripleSec.png"), this, 0.34, 1.0);
            tripleSec->setName(buttonName);
            ingredientsInWorld.push_back(tripleSec);
            tripleSec->show();
        }
        else if (buttonName == "limeJuice"){
            clearPhysicsObjects();
            PhysicsObject* limeJuice = new PhysicsObject(&world, QImage(":/icons/limeJuice.png"), this, 0.26, 0.6);
            limeJuice->setName(buttonName);
            ingredientsInWorld.push_back(limeJuice);
            limeJuice->show();
        }
        else if (buttonName == "agaveSyrup"){
            clearPhysicsObjects();
            PhysicsObject* agaveSyrup = new PhysicsObject(&world, QImage(":/icons/agaveSyrup.png"), this, 0.2, 0.5);
            agaveSyrup->setName(buttonName);
            ingredientsInWorld.push_back(agaveSyrup);
            agaveSyrup->show();
        }
        else if (buttonName == "bitters"){
            clearPhysicsObjects();
            PhysicsObject* bitters = new PhysicsObject(&world, QImage(":/icons/bitters.png"), this, 0.15, 0.5);
            bitters->setName(buttonName);
            ingredientsInWorld.push_back(bitters);
            bitters->show();
        }
        else if (buttonName == "blueCuracao"){
            clearPhysicsObjects();
            PhysicsObject* blueCuracao = new PhysicsObject(&world, QImage(":/icons/blueCuracao.png"), this, 0.15, 0.5);
            blueCuracao->setName(buttonName);
            ingredientsInWorld.push_back(blueCuracao);
            blueCuracao->show();
        }
        else if (buttonName == "coconutCream"){
            clearPhysicsObjects();
            PhysicsObject* coconutCream = new PhysicsObject(&world, QImage(":/icons/coconutCream.png"), this, 0.3, 0.4);
            coconutCream->setName(buttonName);
            ingredientsInWorld.push_back(coconutCream);
            coconutCream->show();
        }
        else if (buttonName == "gin"){
            clearPhysicsObjects();
            PhysicsObject* gin = new PhysicsObject(&world, QImage(":/icons/gin.png"), this, 0.32, 1.0);
            gin->setName(buttonName);
            ingredientsInWorld.push_back(gin);
            gin->show();
        }
        else if (buttonName == "pineappleJuice"){
            clearPhysicsObjects();
            PhysicsObject* pineappleJuice = new PhysicsObject(&world, QImage(":/icons/pineappleJuice.png"), this, 0.25, 0.4);
            pineappleJuice->setName(buttonName);
            ingredientsInWorld.push_back(pineappleJuice);
            pineappleJuice->show();
        }
        else if (buttonName == "sweetVermouth"){
            clearPhysicsObjects();
            PhysicsObject* sweetVermouth = new PhysicsObject(&world, QImage(":/icons/sweetRedVermouth.png"), this, 0.3, 1.0);
            sweetVermouth->setName(buttonName);
            ingredientsInWorld.push_back(sweetVermouth);
            sweetVermouth->show();
        }
        else if (buttonName == "vermouth"){
            clearPhysicsObjects();
            PhysicsObject* vermouth = new PhysicsObject(&world, QImage(":/icons/vermouth.png"), this, 0.28, 1.0);
            vermouth->setName(buttonName);
            ingredientsInWorld.push_back(vermouth);
            vermouth->show();
        }
        else if (buttonName == "whiskey"){
            clearPhysicsObjects();
            PhysicsObject* whiskey = new PhysicsObject(&world, QImage(":/icons/whiskey.png"), this, 0.32, 1.0);
            whiskey->setName(buttonName);
            ingredientsInWorld.push_back(whiskey);
            whiskey->show();
        }
        else if (buttonName == "ice"){
            clearPhysicsObjects();
            PhysicsObject* ice = new PhysicsObject(&world, QImage(":/icons/iceCubes.png"), this, 0.3, 0.3);
            ice->setName(buttonName);
            ingredientsInWorld.push_back(ice);
            ice->show();
        }
        else if (buttonName == "lime"){
            clearPhysicsObjects();
            PhysicsObject* lime = new PhysicsObject(&world, QImage(":/icons/limeSlice.png"), this, 0.2, 0.2);
            lime->setName(buttonName);
            ingredientsInWorld.push_back(lime);
            lime->show();
        }
        else if (buttonName == "saltRim"){
            clearPhysicsObjects();
            PhysicsObject* saltRim = new PhysicsObject(&world, QImage(":/icons/saltRim.png"), this, 0.3, 0.3);
            saltRim->setName(buttonName);
            ingredientsInWorld.push_back(saltRim);
            saltRim->show();
        }
        else if (buttonName == "olive"){
            clearPhysicsObjects();
            PhysicsObject* olive = new PhysicsObject(&world, QImage(":/icons/olives.png"), this, 0.1, 0.2);
            olive->setName(buttonName);
            ingredientsInWorld.push_back(olive);
            olive->show();
        }
        else if (buttonName == "cherry"){
            clearPhysicsObjects();
            PhysicsObject* cherry = new PhysicsObject(&world, QImage(":/icons/maraschinoCherry.png"), this, 0.2, 0.2);
            cherry->setName(buttonName);
            ingredientsInWorld.push_back(cherry);
            cherry->show();
        }
        else if (buttonName == "crushedIce"){
            clearPhysicsObjects();
            PhysicsObject* crushedIce = new PhysicsObject(&world, QImage(":/icons/crushedIce.jpg"), this, 0.2, 0.2);
            crushedIce->setName(buttonName);
            ingredientsInWorld.push_back(crushedIce);
            crushedIce->show();
        }
        else if (buttonName == "whiteRum"){
            clearPhysicsObjects();
            PhysicsObject* whiteRum = new PhysicsObject(&world, QImage(":/icons/whiteRum.png"), this, 0.28, 1.0);
            whiteRum->setName(buttonName);
            ingredientsInWorld.push_back(whiteRum);
            whiteRum->show();
        }
    }
}

void MainWindow::recipeCheckStateChanged(){
    if (ui->recipeCheckBox->isChecked())
        ui->recipeText->hide();
    else
        ui->recipeText->show();
}

void MainWindow::showHelpDialog(){
    introduction.setModal(true);
    introduction.show();
}

// SelectedBodyCallback Methods
MainWindow::SelectedBodyCallback::SelectedBodyCallback(b2Vec2 point) : point(point), body(nullptr) {}

bool MainWindow::SelectedBodyCallback::ReportFixture(b2Fixture* fixture) {
    if (fixture->TestPoint(point)) {
        body = fixture->GetBody();
        return false; // stop after finding the first one
    }
    return true;
}
