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

#ifndef USERDRINK_H
#define USERDRINK_H
#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <QVector>
#include "ingredientData.h"

class UserDrink : public QObject
{
    Q_OBJECT

private:
    QString name;
    int currentStep;
    QVector<IngredientData*> ingredients;

public:

    UserDrink(QObject *parent = nullptr);

    ~UserDrink();

    /**
     * Returns the list of ingredients in this drink.
     * @return The ingredients* QVector
     */
    QVector<IngredientData*>& getIngredients();

    /**
     * Change all actions of the ingredients to SHAKE (unless its a garnish).
     */
    void shakeIngredient();

    /**
     * Adds an ingredient to the drink.
     * @param ingredientName - the name of the added ingredient
     * @param amount - the amount added in oz (0 for dash/garnish)
     */
    void addIngredient(QString ingredientName, float amount, bool isGarnish);
};

#endif // USERDRINK_H
