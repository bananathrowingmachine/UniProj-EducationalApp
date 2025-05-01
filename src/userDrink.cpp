/**
 * Represents the user's drink in the model. Mostly used for scoring their drink.
 *
 * 
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 *
 * Checked by [redacted]
 * April 22, 2025
 **/

#include "userDrink.h"
#include <QDebug>

UserDrink::UserDrink(QObject *parent) : QObject{parent} {}

UserDrink::~UserDrink(){
    qDeleteAll(ingredients);
    ingredients.clear();
}

void UserDrink::addIngredient(QString ingredientName, float amount, bool isGarnish){
    if(ingredientName.isEmpty()){
        qWarning() << "Attempted to add empty ingredient name";
        return;
    }

    ingredients.append(new IngredientData(ingredientName, amount, isGarnish ? Action::GARNISH : Action::NOTHING));
}

void UserDrink::shakeIngredient(){
    if(ingredients.isEmpty()) return;

    for (IngredientData* ingredient : ingredients) {
        if (ingredient->getAction() != Action::GARNISH)
            ingredient->setAction(Action::SHAKE);
    }
}

QVector<IngredientData*>& UserDrink::getIngredients(){
    return ingredients;
}
