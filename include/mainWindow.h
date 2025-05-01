/**
 * The main view window of the program. Maintains the model object, connecting it with user inputs, as well as organizing everything related in inputs and outputs.
 * Contains a lot of sound systems, images, collision detection, signals, slots, and connect methods that link everything together.
 *
 * 
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 * April 22, 2025
 **/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMouseEvent>
#include "physicsObject.h"
#include <QVector>
#include <QSoundEffect>
#include <QElapsedTimer>
#include "measureDialog.h"
#include "jiggerTool.h"
#include "scoreWindow.h"
#include "model.h"
#include <QLabel>
#include <QPixmap>
#include "pourwindow.h"
#include "introductionwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    class SelectedBodyCallback : public b2QueryCallback {
    public:
        b2Vec2 point;
        b2Body* body;

        SelectedBodyCallback(b2Vec2 point);
        bool ReportFixture(b2Fixture* fixture) override;
    };

    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    MeasureDialog measure;
    ScoreWindow scoreWindow;
    PourWindow pourDialog;
    IntroductionWindow introduction;
    model modelClass;
    bool garnishNeedsDeleted = false;

    // Box2D variables/objects
    b2World world;
    b2Body* body;
    QTimer* physicsTimer;
    b2Body* groundBody;
    b2Body* leftWallBody;
    b2Body* rightWallBody;
    b2MouseJoint* mouseJoint = nullptr;
    b2Vec2 target;

    // Drink variables/objects
    PhysicsObject* mixerObj;
    PhysicsObject* jiggerObj;
    PhysicsObject* glassObj;
    QList<PhysicsObject*> ingredientsInWorld;
    QList<PhysicsObject*> drinkTools;
    JiggerTool jiggerModel;
    QList<QString> mixerTooltipElements;
    bool mixerIsShaken = false;

    // SFX variables/objects
    QList<QSoundEffect*> activeSounds;
    QElapsedTimer collisionSoundRateLimiter;
    QElapsedTimer shakeSoundRateLimiter;
    QSoundEffect* shakeSFX;
    QSoundEffect* pourSFX;
    int currentStepIndex;
    QStringList instructionSteps;

    int consecutiveShakes;

    /**
     * Creates a joint an attatches it to a clicked object to allow for dragging
     * @param event - Mouse event (Used to get the position of the mouse)
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * Updates the position of the mouse joint to simulate dragging (if it exists)
     * @param event - Mouse event (Used to get the position of the mouse)
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * Calls for the mouse joint to be destroyed
     * @param event - Mouse event (Unused!)
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * Destroys the mouse joint used for dragging (if it exists)
     */
    void destroyMouseJoint();

    /**
     * Deletes all 'bottle' ingredient objects
     */
    void clearPhysicsObjects();

    /**
     * Updates the tooltip of the mixer to show the Mixer contents
     */
    void updateMixerTooltip();

    /**
     * Adds the given garnish and image label to the garnish layout window
     * @param garnish - Garnish
     * @param labelImage - Garnish image label
     */
    void addItemToLayout(QString garnish);

    /**
     * Shows the next step for the current recipe.
     */
    void onNextClicked();

    /**
     * Shows the previous step for the current recipe.
     */
    void onBackClicked();

    void shakeMixerView();

public slots:
    /**
     * Updates all collision objects
     */
    void updateWorld();

    /**
     * Spawns the ingredient corresponding to the pushed button in the UI
     */
    void ingredientAppears();

    /**
     * Resets the positions of the Glass, Jigger, and Mixer
     */
    void resetPositions();

    /**
     * Shows the measuring dialogue window
     */
    void measureDialogOpened();

    /**
     * Destroys the dragging mouse joint the the attatched ingredient
     */
    void measureDialogClosed();

    /**
     * Destroys the dragging mouse joint the the attatched ingredient,
     * updates the Jigger tooltip with the given ingredient amount and name,
     * and plays the pouring sound
     * @param amount - Measured ingredient amount
     * @param ingredientName - Measured ingredient name
     */
    void ingredientMeasuredWithJigger(float amount, const QString& ingredientName);

    /**
     * Adds the contents of the Jigger to the Mixer, clears Jigger contents,
     * updates the Mixer tootlip, and plays the pouring sound
     */
    void pourJiggerIntoMixer();

    /**
     * Adds either the special bitters or ice into the mixer, and update the Mixer tootlip
     */
    void pourDirectlyIntoMixer(QString name, QString tooltip);

    /**
     * Plays a sound when a bottle collides with an object
     */
    void playCollisionSound();

    /**
     * Pours the Mixer contents into the glass, and readies the drink to accept garnishes
     */
    void pouredMixerIntoGlass();

    /**
     * Emits for the UI to display the chosen recipe
     */
    void recipeChosen();

    /**
     * Diplays the chosen recipe
     * @param recipe - Recipe to display
     */
    void displayRecipe(QStringList recipe);

    /**
     * Hides the score window, resets all tooltips,
     * resets the current recipe, and clears the recipe list
     */
    void restartGame();

    /**
     * Opens the score Window and emits for the given score to be displayed
     * @param score - score to display
     */
    void displayUserScore(int score, QString explination);

    /**
     * Deletes everything in a layout
     * @param layout - the layout to be cleared
     */
    void clearLayout(QLayout* layout);

    /**
     * Allows the user to pour liquid from the mixer to the glass.
     */
    void pouringDrink(QString object1, QString object2);

    /**
     * Opens the pouring dialog window.
     */
    void openPouringDialog(QString object1, QString object2);
    /**
     * Emitted when a collision occurs between a Garnish and the glass
     * @param garnish - Garnish Name
     */
    void addGarnishToLayout(QString garnish);

    /**
     * Handles if the window is closed.
     */
    void pourWindowClosed();

    /**
     * Handles the check box state for the recipe text.
     */
    void recipeCheckStateChanged();

    /**
     * Handles the help button.
     */
    void showHelpDialog();

signals:
    /**
     * Emitted when Jigger ingredients need to be added to the Mixer
     * @param ingredientName - Jigger ingredient name
     * @param amount - Jigger ingredient amount
     * @param isGarnish - Is the ingredient a garnish? (is this even used?)
     */
    void addIngredientToDrinkModel(QString ingredientName, float amount, bool isGarnish);

    /**
     * Emitted when the user is shaking the Mixer
     */
    void shakingMixer();

    /// [ Currently Unimplemented ] DO THIS!!!!
    void addToGlass(QString ingredientName);

    /**
     * Emitted when the recipe is chosen
     */
    void showSelectedDrink(QString);

    /**
     * Sends the score to the dialog
     * @param score - the users score
     */
    void sendScoreToDialog(int score, QString explination);

    /**
     * Emitted when a collision occurs between the Mixer and the Glass
     */
    void pourMixerIntoGlass();

    void sendStringsToPourDialog(QString object1, QString object2);

};


#endif // MAINWINDOW_H
