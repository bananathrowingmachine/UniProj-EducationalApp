#include "ingredientcontactlistener.h"
#include <QApplication>

// IngredientContactListener::IngredientContactListener() {}

void IngredientContactListener::BeginContact(b2Contact* contact) {
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    void* userDataA = fixtureA->GetBody()->GetUserData();
    void* userDataB = fixtureB->GetBody()->GetUserData();

    PhysicsObject* objA = static_cast<PhysicsObject*>(userDataA);
    PhysicsObject* objB = static_cast<PhysicsObject*>(userDataB);

    if (objA && objB) {
        if(PhysicsObject* jiggerObj = collisionWith(objA, objB, "Jigger")){
            // Detect collision with jigger and mixer
            if(collisionWith(objA, objB, "Mixer")){
                emit mixerJiggerCollision();
            }
            // Collision with jigger and a liquid ingredient
            else if(!collisionWith(objA, objB, "Glass") && !collisionWith(objA, objB, "MainWindow") && !collisionWith(objA, objB, "bitters") && !isGarnishCollision(objA, objB)){
                emit measureOpened(jiggerObj == objA ? makeProper(objB->objectName()) : makeProper(objA->objectName()));
            }
        }

        // Detect collision with mixer and glass
        else if(collisionWith(objA, objB, "Mixer", "Glass")){
            emit callPouringDialog("Mixer" , "Glass");
        }

        // Detect collision glass and garnishes
        else if(PhysicsObject* garnishObj = collisionWith(objA, objB, "garnish", "Glass")) {
            QString garnishName = garnishObj->objectName();
            // Immediately mark the garnish object for deletion
            garnishObj->markForDeletion();
            // Delay the signal emission to ensure physics update completes
            QMetaObject::invokeMethod(qApp, [this, garnishName]() {
                emit addGarnishToGlass(garnishName);
            }, Qt::QueuedConnection);
        }
        else if(PhysicsObject* bittersObj = collisionWith(objA, objB, "bitters", "Mixer"))
        {
            bittersObj->markForDeletion();
            emit callPouringDialog("Bitters" , "Mixer");
        }
        else if(PhysicsObject* iceObj = collisionWith(objA, objB, "ice", "Mixer"))
        {
            iceObj->markForDeletion();
            emit callPouringDialog("Ice" , "Mixer");
        }
        emit collisionOccured();
    }
}

bool IngredientContactListener::isGarnishCollision(PhysicsObject* objA, PhysicsObject* objB) {
    return getGarnishName(objA, objB) != "";
}

QString IngredientContactListener::getGarnishName(PhysicsObject* objA, PhysicsObject* objB) {
    QStringList garnishes = {"ice", "lime", "saltRim", "olive", "cherry", "crushedIce"};
    QString objAname = objA->objectName();
    QString objBname = objB->objectName();

    if (garnishes.contains(objAname))
        return objAname;
    if (garnishes.contains(objBname))
        return objBname;
    return ""; // No garnish found
}

QString IngredientContactListener::makeProper(QString name) {
    // Add as space where all camelCase lettters are
    int size = name.size();
    QString properName = name;

    for (int i = 0; i < size; i++) {
        QChar letter = properName.at(i);
        // If character is uppercase
        if (letter.isUpper()) {
            properName.insert(i, " ");
            size++;
            i++;
        }
    }

    // Set properName to all lowercase
    return properName.toLower();
}

PhysicsObject* IngredientContactListener::collisionWith(PhysicsObject* objA, PhysicsObject* objB, QString name, QString otherName){
    if (name == "garnish")
        name = getGarnishName(objA, objB);
    QList<QString> names = {objA->objectName(), objB->objectName()};
    if (names.contains(name) && (otherName == "" || names.contains(otherName)))
        return (names[0] == name) ? objA : objB;
    return nullptr;
}
