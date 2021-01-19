import QtQuick 2.9
import QtPurchasing 1.12

import "qrc:/js/payment.js" as AP

Store {
    id: sStore

    Product {
        id: pStoreIntem0
        identifier: "shop_item_gold_0"
        type: Product.Consumable
        property bool purchasing: false

        onPurchaseSucceeded: AP.purchaseSucceeded(item, transaction)
        onPurchaseFailed: AP.purchaseFailed(item, transaction)
        onPurchaseRestored: AP.purchaseRestored(item, transaction)
        onStatusChanged: AP.productStatusChanged(status, identifier, price, 0)
    }

    Product {
        id: pStoreIntem1
        identifier: "shop_item_gold_1"
        type: Product.Consumable
        property bool purchasing: false

        onPurchaseSucceeded: AP.purchaseSucceeded(item, transaction)
        onPurchaseFailed: AP.purchaseFailed(item, transaction)
        onPurchaseRestored: AP.purchaseRestored(item, transaction)
        onStatusChanged: AP.productStatusChanged(status, identifier, price, 1)
    }

    Product {
        id: pStoreIntem2
        identifier: "shop_item_gold_2"
        type: Product.Consumable
        property bool purchasing: false

        onPurchaseSucceeded: AP.purchaseSucceeded(item, transaction)
        onPurchaseFailed: AP.purchaseFailed(item, transaction)
        onPurchaseRestored: AP.purchaseRestored(item, transaction)
        onStatusChanged: AP.productStatusChanged(status, identifier, price, 2)
    }

    Product {
        id: pStoreIntem3
        identifier: "shop_item_gold_3"
        type: Product.Consumable
        property bool purchasing: false

        onPurchaseSucceeded: AP.purchaseSucceeded(item, transaction)
        onPurchaseFailed: AP.purchaseFailed(item, transaction)
        onPurchaseRestored: AP.purchaseRestored(item, transaction)
        onStatusChanged: AP.productStatusChanged(status, identifier, price, 3)
    }

    Product {
        id: pStoreIntem4
        identifier: "shop_item_gold_4"
        type: Product.Consumable
        property bool purchasing: false

        onPurchaseSucceeded: AP.purchaseSucceeded(item, transaction)
        onPurchaseFailed: AP.purchaseFailed(item, transaction)
        onPurchaseRestored: AP.purchaseRestored(item, transaction)
        onStatusChanged: AP.productStatusChanged(status, identifier, price, 4)
    }

    function buyGold(idx) {
        AP.buyGold(idx);
    }
}
