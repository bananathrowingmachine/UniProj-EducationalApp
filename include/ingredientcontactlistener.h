#ifndef INGREDIENTCONTACTLISTENER_H
#define INGREDIENTCONTACTLISTENER_H

#include <QObject>
#include <Box2D/Box2D.h>
#include "physicsObject.h"

class IngredientContactListener : public QObject, public b2ContactListener
{
    Q_OBJECT

public:
    /**
     * Detects collisions between objects and emits the corresponding signals
     * @param contact - Contact event
     */
    void BeginContact(b2Contact* contact) override;

private:
    /**
     * Determines if an ingredient is involved in the collision of two ingredient objects. Input "garnish" in to check for all garnishes
     * @param objA - Ingredient Object #1
     * @param objB - Ingredient Object #2
     * @param name - Ingredient to check for
     * @param otherName - Other ingredient to check for, defaults to ""
     * @return The physObj pointer to the obj inputted that matches with "name". Returns nullptr if "name" and "otherName" don't match
     */
    PhysicsObject* collisionWith(PhysicsObject* objA, PhysicsObject* objB, QString name, QString otherName = "");

    /**
     * Determines if the collision of two ingredient objects contains a garnish
     * @param objA - Ingredient Object #1
     * @param objB - Ingredient Object #2
     * @return True if the collision contains a garnish; false otherwise
     */
    bool isGarnishCollision(PhysicsObject* objA, PhysicsObject* objB);

    /**
     * Gets the name of the garnish involved in the collision (if there is one)
     * @param objA - Ingredient Object #1
     * @param objB - Ingredient Object #2
     * @return The name of the garnish (if there is one)
     */
    QString getGarnishName(PhysicsObject* objA, PhysicsObject* objB);

    /**
     * Takes the given string and returns the proper version of it
     * @param name - Ingredient Name
     * @return Name made proper
     */
    QString makeProper(QString name);



signals:
    /**
     * Emitted when a collision occurs between the Jigger and a liquid ingredient
     * @param ingredientName - Liquid ingredient name
     */
    void measureOpened(QString ingredientName);

    /**
     * Emitted when a collision occurs between the Mixer and the Jigger
     */
    void mixerJiggerCollision();

    /**
     * Emitted when a collision occurs between the Mixer and the special cases Ice and Bitters
     * @param name - name of the other colliding object
     * @param tooltip - the tooltip to add
     */
    void mixerSpecialCollision(QString name, QString tooltip);

    /**
     * Emitted when a collision occurs between any two objects
     */
    void collisionOccured();

    /**
     * Emitted when a collision occurs between an ingredient and the Glass
     * @param ingredientName - Ingredient name
     * @param amount - Ingredient amount
     * @param isGarnish - Is the ingredient a garnish? (Is this even used?)
     */
    void addIngredient(QString ingredientName, float amount, bool isGarnish);

    /**
     * Emitted when a collision occurs between a Garnish and the glass
     * @param garnish - Garnish Name
     */
    void addGarnishToLayout(QString garnishName);

    /**
     * Telling the view to show the dialog for pouring.
     */
    void callPouringDialog(QString object1, QString object2);



    void addGarnishToGlass(QString garnish);
};

#endif // INGREDIENTCONTACTLISTENER_H
