/**
 * Represents physics objects in the view. Works with Box2D to allow them to get thrown around. Not used for the model.
 *
 * 
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 * April 22, 2025
 **/

#include "physicsObject.h"

PhysicsObject::PhysicsObject(QWidget *parent) : QLabel{parent} {}

PhysicsObject::PhysicsObject(b2World *world, QImage image, QWidget *parent, float width, float height, bool isStatic) : QLabel{parent}, image(image)
{
    this->bWidth = width;
    this->bHeight = height;
    float scale = 100.0f;

    // Define the dynamic body. We set its position and call the body factory.
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    if(isStatic)
        bodyDef.type = b2_staticBody;

    bodyDef.position.Set(500 / scale, 200 / scale);
    body = world->CreateBody(&bodyDef);
    body->SetUserData(this);

    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(bWidth, bHeight);

    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;


    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;
    // Override the default friction.
    fixtureDef.friction = 0.3f;

    // fixtureDef.restitution = 0.9f;
    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);

    setCursor(Qt::OpenHandCursor);

    // Set the mass data of body
    // if(bodyDef.type == b2_dynamicBody){
    //     myMassData=new b2MassData();
    //     myMassData->mass = 10.0f;
    //     myMassData->center = (b2Vec2){0, 0};
    //     myMassData->I = 100.0f;
    //     body->SetMassData(myMassData);
    // }
}

PhysicsObject::~PhysicsObject(){
    body = nullptr;
}

void PhysicsObject::updateObject(int worldHeight){
    float scale = 100.0f;

    // Get Box2D body position
    float x = body->GetPosition().x * scale;
    float y = worldHeight - (body->GetPosition().y * scale); // Flip Y

    // Get angle in degrees
    float angleRadians = body->GetAngle();
    float angleDegrees = angleRadians * (180.0f / M_PI);

    // --- STEP 1: Compute desired size in pixels based on body size ---
    float boxWidth = (bWidth*2) * scale;  // 0.5 + 0.5 meters = 1m → 100px
    float boxHeight = (bHeight*2) * scale; // 1.0 + 1.0 meters = 2m → 200px

    // 1.05f in this case is to make the label slightly bigger than the collision box so collisions look better
    QSize targetSize(static_cast<int>(boxWidth * 1.05f), static_cast<int>(boxHeight * 1.05f));

    // --- STEP 2: Scale the image to desired size ---
    QImage scaledImage = image.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // --- STEP 3: Rotate the scaled image ---
    QTransform transform;
    transform.rotate(360-angleDegrees);
    QPixmap rotated = QPixmap::fromImage(scaledImage.transformed(transform, Qt::SmoothTransformation));

    setPixmap(rotated);
    setScaledContents(false);
    setFixedSize(rotated.size());

    // --- STEP 4: Position QLabel so it's centered on the Box2D body ---
    setGeometry(x - width() / 2, y - height() / 2, width(), height());
}

void PhysicsObject::setPosition(float x, float y){
    body->SetTransform(b2Vec2(x, y), body->GetAngle());
}

void PhysicsObject::setName(const QString& name) {
    this->setObjectName(name);
}

void PhysicsObject::setImage(QImage image){
    this->image = image;
}

void PhysicsObject::markForDeletion(){
    markedForDeletion = true;
}

bool PhysicsObject::isMarkedForDeletion(){
    return markedForDeletion;
}


