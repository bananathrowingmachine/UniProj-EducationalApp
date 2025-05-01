/**
 * Maintains drink recipes. Stores there contents, builds a string recipe for the view, and will compare itself against a list of ingredients to rank the user.
 *
 * 
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 *
 * Checked by [redacted]
 * April 22, 2025
 **/

#ifndef RECIPE_H
#define RECIPE_H
#include <QString>
#include <QVector>
#include <QMap>
#include "ingredientData.h"

class Recipe
{
private:
    QString name;
    int currentStep;
    QVector<IngredientData*> ingredients;
    QStringList recipeString;
    QString rankString;

public:
    Recipe();

    ~Recipe();

    Recipe(QVector<IngredientData*> ingredients, QString officialName);

    /**
     * Returns a string representing the recipe instructions
     * @return Recipe instructions
     */
    QStringList getRecipeString();

    /**
     * Compares the calling recipe to the given ingredient list and ranks the accuracy out of 10.
     * Assumes 'otherIngredients' is the user's recipe
     * @param otherIngredients - The list of other ingredients to compare accuracy to.
     * @return The rank of accuracy out of 10
     */
    int compareRecipes(const QVector<IngredientData*>& otherIngredients);

    /**
     * Gets the ingredients in the recipe
     * @return The ingredients
     */
    QVector<IngredientData*> getIngredients();

    /**
     * Returns the string form of the given action.
     * Used by compareRecipes
     * @param action - Given action
     * @return String form of the action
     */
    QString actionToString(Action action);

    /**
     * Gets the string explaining the score.
     * @return
     */
    QString getRankString();
};

#endif // RECIPE_H
