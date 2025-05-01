/**
 * Represents physics objects in the view. Works with Box2D to allow them to get thrown around. Not used for the model.
 *
 * 
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 * April 22, 2025
 **/

#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include <QLabel>
#include <Box2D/Box2D.h>
#include <QPixmap>

class PhysicsObject : public QLabel
{
    Q_OBJECT
private:
    bool markedForDeletion = false;
    b2MassData* myMassData;
public:
    b2Body* body;
    QImage image;
    float bWidth;
    float bHeight;

    void markForDeletion();
    bool isMarkedForDeletion();

    explicit PhysicsObject(QWidget *parent = nullptr);

    PhysicsObject(b2World *world, QImage image, QWidget *parent = nullptr, float width = 0.5f, float height = 1.0f, bool isStatic = false);

    ~PhysicsObject();

    /**
     * Visually updates the label tied to the object
     * @param worldHeight - World Height
     */
    void updateObject(int worldHeight);

    /**
     * Sets the position of the object to the given coordinates
     * @param x - x coordinate
     * @param y - y coordinate
     */
    void setPosition(float x, float y);

    /**
     * Sets the name of the object to the given name
     * @param name - Object name
     */
    void setName(const QString& name);

    /**
     * Sets the image of the object to the given image
     * @param image - Object image
     */
    void setImage(QImage image);
};

#endif // PHYSICSOBJECT_H
