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

#include "model.h"
#include <QVector>
#include <QString>

model::model() {
    createRecipes();
    currentDrink = nullptr;
    currentRecipe = nullptr;
}

model::~model() {
    delete currentDrink;
    delete currentRecipe;
    qDeleteAll(recipeList);
    recipeList.clear();
}

void model::startRound(QString recipeName){
    currentRecipe = recipeList[recipeName];
    delete currentDrink;
    currentDrink = new UserDrink();
    if (currentRecipe != nullptr)
        emit sendRecipe(currentRecipe->getRecipeString());
}

void model::submitDrink(){
    if(currentRecipe && currentDrink) {
        // Use const reference to avoid copying
        const QVector<IngredientData*>& userIngredients = currentDrink->getIngredients();
        int score = currentRecipe->compareRecipes(userIngredients);
        // emit sendingUserScore(score)
        emit sendingUserScore(score, currentRecipe->getRankString());
    }
}

void model::createRecipes() {
    // Create and add 'Margarita' recipe
    QVector<IngredientData*> margaritaIngredients = {
        new IngredientData(QString("tequila"), 2, Action::SHAKE),
        new IngredientData(QString("tripleSec"), 1, Action::SHAKE),
        new IngredientData(QString("limeJuice"), 1, Action::SHAKE),
        new IngredientData(QString("ice"), 0, Action::SHAKE),
        new IngredientData(QString("saltRim"), 0, Action::GARNISH)
    };

    Recipe* margaritaRecipe = new Recipe(margaritaIngredients, "Margarita");
    recipeList.insert(QString("Margarita"), margaritaRecipe);

    // Create and add 'Martini' recipe
    QVector<IngredientData*> martiniIngredients = {
        new IngredientData(QString("gin"), 2.5, Action::SHAKE),
        new IngredientData(QString("vermouth"), 0.5, Action::SHAKE),
        new IngredientData(QString("ice"), 0, Action::GARNISH),
        new IngredientData(QString("olive"), 0,  Action::GARNISH)
    };

    Recipe* martiniRecipe = new Recipe(martiniIngredients, "Martini");
    recipeList.insert(QString("Martini"), martiniRecipe);

    // Create and add 'Pina Colada' recipe
    QVector<IngredientData*> pinaColadaIngredients = {
        new IngredientData(QString("pineappleJuice"), 3),
        new IngredientData(QString("whiteRum"), 1),
        new IngredientData(QString("coconutCream"), 1, Action::SHAKE),
        new IngredientData(QString("crushedIce"), 0, Action::GARNISH)
    };

    Recipe* pinaColadaRecipe = new Recipe(pinaColadaIngredients, "Pina Colada");
    recipeList.insert(QString("Pina Colada"), pinaColadaRecipe);

    // Create and add 'Manhattan' recipe
    QVector<IngredientData*> manhattanIngredients = {
        new IngredientData(QString("whisky"), 2, Action::SHAKE),
        new IngredientData(QString("whiteRum"), 1, Action::SHAKE),
        new IngredientData(QString("sweetVermouth"), 0.75, Action::SHAKE),
        new IngredientData(QString("bitters"), 0, Action::SHAKE),
        new IngredientData(QString("cherry"), 0, Action::GARNISH)
    };

    Recipe* manhattanRecipe = new Recipe(manhattanIngredients, "Manhattan");
    recipeList.insert(QString("Manhattan"), manhattanRecipe);

    // Create and add 'Blue Blazer' recipe
    QVector<IngredientData*> blueBlazerIngredients = {
        new IngredientData(QString("tequila"), 1.5, Action::SHAKE),
        new IngredientData(QString("blueCuracao"), 0.5, Action::SHAKE),
        new IngredientData(QString("limeJuice"), 0.5, Action::SHAKE),
        new IngredientData(QString("agaveSyrup"), 0.5, Action::SHAKE),
    };

    Recipe* blueBlazerRecipe = new Recipe(blueBlazerIngredients, "Blue Blazer");
    recipeList.insert(QString("Blue Blazer"), blueBlazerRecipe);
}

void model::addIngredient(QString ingredientName, float amount, bool isGarnish) {
    if(!currentDrink)
        return;

    currentDrink->addIngredient(ingredientName, amount, isGarnish);
}

void model::shakeIngredient() {
    if(currentDrink)
        currentDrink->shakeIngredient();
}
