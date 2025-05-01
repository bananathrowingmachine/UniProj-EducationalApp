/**
 * Represents an ingredient for a drink in the model. Used to make the official recipes and also for the user's drink in the model.
 *
 * 
 *
 * Created by [redacted], [redacted], [redacted], bananathrowingmachine, and [redacted]
 * April 22, 2025
 **/

#include "ingredientData.h"
#include <QTextStream>

IngredientData::IngredientData() {}

IngredientData::IngredientData(QString name, double amount, Action action) : name(name), amount(amount), action(action) {
    properName = nullptr;
    makeProperName();
}

IngredientData::~IngredientData() {

}

QString IngredientData::getName() {return name;}

double IngredientData::getAmount() {return amount;}

Action IngredientData::getAction() {return action;}

void IngredientData::setAction(Action action){
    this->action = action;
}

void IngredientData::makeProperName() {
    // Add as space where all camelCase lettters are
    int size = name.size();
    properName = name;

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
    properName = properName.toLower();
}


QString IngredientData::getProperName() {
    return properName;
}

QString IngredientData::createStepString() {
    if (action == Action::GARNISH) {
        if (name == "saltRim")
            return QString("Cover the rim of the glass with salt.");
        if (name == "ice")
            return QString("Fill the glass with ice.");
        if (name == "crushedIce")
            return QString("Fill the glass with crushed ice.");
        else {
            QString result;
            QTextStream stream(&result);
            stream << "Add a " << getProperName() << " to the edge of the glass.";
            return result;
        }
    }
    else if (name == "ice"){
        return QString("Fill the mixer with ice.");
    }
    else if (name == "bitters"){
        return QString("Add a dash of bitters.");
    }
    else {
        QString result;
        QTextStream stream(&result);
        stream << "Pour " << amount << " oz of " << getProperName() << " into the mixer.";
        return result;
    }
}
