/**
 * The backend model for the program. Maintains all the logic, and interacts with the view through signals and slots.
 * Used for ingredient data, building and maintaining the recipes list and their associated recipe strings, storing and scoring the user's drink.
 *
 * 
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 *
 * Checked by [redacted]
 * April 22, 2025
 **/

#ifndef MODEL_H
#define MODEL_H
#include "recipe.h"
#include "userDrink.h"
#include <QMap>

class model : public QObject
{
    Q_OBJECT
private:
    QMap<QString, Recipe*> recipeList;
    Recipe* currentRecipe;
    UserDrink* currentDrink;

public:
    model();

    ~model();

    /**
     * Creates all drink recipes the player can make
     */
    void createRecipes();

public slots:
    /**
     * Starts a new round for the model:
     * Will change the recipe, reset the model's drink, and if a known recipe is specified will emit that recipe to the view.
     * Input any non recipeName string to set the current recipe or none and not have a recipe string get emitted.
     * @param recipeName - the new recipe to now be used
     */
    void startRound(QString recipeName);

    /**
     * Tells the model to submit the stored drink model to the recipe for scoring.
     */
    void submitDrink();

    /**
     * Adds an ingredient to the model's stored user drink.
     * @param ingredientName - the ingredient's name
     * @param amount - the amount
     * @param isGarnish - if it is a garnish
     */
    void addIngredient(QString ingredientName, float amount, bool isGarnish);

    /**
     * Informs the model that the mixer was shaken, where internally it will set the ingredient to Action::SHAKE, which is used in scoring.
     */
    void shakeIngredient();

signals:
    /**
     * Emitted after the recipe string is created
     * @param recipeString - Recipe in the form of instrucions
     */
    void sendRecipe(QStringList recipeString);

    /**
     * Emitted after the user's drink is compared to the actual recipe.
     * @param score - score the user earned.
     */
    void sendingUserScore(int score, QString explination);
};

#endif // MODEL_H
