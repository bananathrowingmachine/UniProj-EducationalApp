/**
 * Represents an ingredient for a drink in the model. Used to make the official recipes and also for the user's drink in the model.
 *
 * 
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 * April 22, 2025
 **/

#ifndef INGREDIENTDATA_H
#define INGREDIENTDATA_H
#include <QString>

enum class Action {NOTHING, SHAKE, GARNISH};

class IngredientData
{

private:
    QString name;
    double amount;
    Action action;
    QString properName;

public:
    IngredientData();

    ~IngredientData();

    IngredientData(QString name, double amount, Action action = Action::NOTHING);

    /**
     * Returns the internal name of the ingredient
     * @return IngredientData Name
     */
    QString getName();

    /**
     * Returns the double amount (oz.) of the ingredient
     * @return IngredientData amount (oz.)
     */
    double getAmount();

    /**
     * Returns the action of the IngredientData
     * @return IngredientData Action {NOTHING, SHAKE, GARNISH}
     */
    Action getAction();

    /**
     * Sets the action of the ingredient
     * @param action - IngredientData action {NOTHIGN, SHAKE, GARNISH}
     */
    void setAction(Action action);

    /**
     * Returns the ingredient as a set of recipe instructions
     * @return creates what do to with this ingredient for each step
     */
    QString createStepString();

    /**
     * Creates the proper name of the ingredient, if it need one
     */
    void makeProperName();

    /**
     * Gets the proper name of the ingredient.
     * Returns the name if it has no proper name
     * @return Proper name
     */
    QString getProperName();
};

#endif // INGREDIENTDATA_H
