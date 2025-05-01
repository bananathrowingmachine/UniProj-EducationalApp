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

#include "recipe.h"
#include <QTextStream>
#include <QDebug>

Recipe::Recipe() {}

Recipe::~Recipe() {
    qDeleteAll(ingredients);
    ingredients.clear();
}

Recipe::Recipe(QVector<IngredientData*> ingredients, QString officialName) : ingredients(ingredients) {
    recipeString << "Recipe for: " + officialName ;

    int index = 1;

    for (int i = 0; i < ingredients.count(); i++) {
        IngredientData* ingredient = ingredients[i];
        recipeString << "Step " + QString::number(index) + ": " + ingredient->createStepString();

        // If this ingredient is that last ingredient added to the mixer before shaking
        if (ingredient->getAction() == Action::SHAKE && (i+1 < ingredients.count() && ingredients[i+1]->getAction() != Action::SHAKE)) {
            index++;
            recipeString << "Step " + QString::number(index) + ": Shake the mixer.";
        }
        index++;
    }

    recipeString << "Step " + QString::number(index) + ": Pour contents into the glass.";
    index++;
    recipeString << "Step " + QString::number(index) + ": Serve!";
}

QStringList Recipe::getRecipeString() {return recipeString;}

int Recipe::compareRecipes(const QVector<IngredientData*>& otherIngredients){
    rankString.clear();
    // If the recipe contains no ingredients, you get no points
    if (otherIngredients.empty()) {
        rankString = "You submitted and empty drink!";
        return 0;
    }

    // Names of the ingredients in the recipe
    QVector<QString> ingredientNames;
    for (IngredientData* ingredient : std::as_const(ingredients)) {
        ingredientNames.append(ingredient->getProperName());
    }

     // Names of the ingredients in the player recipe
    QVector<QString> otherIngredientNames;
    for (IngredientData* ingredient : std::as_const(otherIngredients)) {
        otherIngredientNames.append(ingredient->getProperName());
    }

    // The recipe rank out of 10
    int score = 10;
    // If the other recipe is in the correct order
    bool correctOrder = true;

    for (int i = 0; i < ingredients.count(); i++) {
        IngredientData* selfIngredient = ingredients[i];
        qDebug() << "self Ingredient: " << selfIngredient->getName();

        for (int j = 0; j < otherIngredients.count(); j++) {
            IngredientData* otherIngredient = otherIngredients[j];
             qDebug() << "other Ingredient: " << otherIngredient->getName();

            if (selfIngredient->getProperName() == otherIngredient->getProperName()) {
                QString name = otherIngredient->getProperName();
                // The ingredients do not appear in the same order
                if (i != j)
                    correctOrder = false;
                // If the amounts differ, -1 point
                if  (selfIngredient->getAmount() != otherIngredient->getAmount()) {
                    double wrongAmount = otherIngredient->getAmount();
                    double rightAmount = selfIngredient->getAmount();

                    rankString += QString("You added %1 oz. of %2, instead of %3 oz. (-1 pt)\n").arg(wrongAmount).arg(name).arg(rightAmount);
                    score--;
                }
                // If the ingredient actions do not match, -1 point
                if (selfIngredient->getAction() != otherIngredient->getAction()) {
                    Action wrongAction = otherIngredient->getAction();
                    Action rightAction = selfIngredient->getAction();

                    rankString += QString("The %1 was %2, not %3. (-1 pt)\n").arg(name).arg(actionToString(wrongAction)).arg(actionToString(rightAction));
                    score--;
                }
            }
        }

        // If the recipe is missing an ingredient, -2 points
        if (!otherIngredientNames.contains(selfIngredient->getProperName())) {
            rankString += QString("The recipe is missing %1. (-2 pts)\n").arg(selfIngredient->getProperName());
            score -= 2;
        }
    }

    // Check if user recipe contains any extra ingredients
    for (int i = 0; i < otherIngredients.count(); i++) {
        IngredientData* otherIngredient = otherIngredients[i];
        // If an ingredient is not featured in the recipe, -2 points
        if (!ingredientNames.contains(otherIngredient->getProperName())) {
            rankString += QString("This recipe does not contain %1. (-2 pts)\n").arg(otherIngredient->getProperName());
            score -= 2;
        }
    }

    // If the recipe is not in correct order, -2 points
    if (!correctOrder) {
        rankString += "Ingredients were added in the wrong order. (-2 pts)\n";
        score -= 2;
    }

    // If the score is less than zero, just return zero
    return score < 0 ? 0 : score;
}

QVector<IngredientData*> Recipe::getIngredients() {
    return ingredients;
}

QString Recipe::actionToString(Action action) {
    switch (action) {
        case Action::SHAKE:     return "shaken";
        case Action::GARNISH:   return "garnished";

        default: return "added";
    }
}

QString Recipe::getRankString(){
    return rankString;
}
