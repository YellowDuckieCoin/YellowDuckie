// Copyright (c) 2011-2014 The Bitcoin Core developers
// Copyright (c) @2024 The YellowDuckieCoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef YellowDuckieCoin_QT_TRANSACTIONDESC_H
#define YellowDuckieCoin_QT_TRANSACTIONDESC_H

#include <QObject>
#include <QString>

class TransactionRecord;

class CWallet;
class CWalletTx;

/** Provide a human-readable extended HTML description of a transaction.
 */
class TransactionDesc: public QObject
{
    Q_OBJECT

public:
    static QString toHTML(CWallet *wallet, CWalletTx &wtx, TransactionRecord *rec, int unit);
    static QString toAssetHTML(CWallet *wallet, CWalletTx &wtx, TransactionRecord *rec, int unit);
    static void CreateDebugString(QString& strHTML, CWallet *wallet, CWalletTx &wtx, int unit);

private:
    TransactionDesc() {}

    static QString FormatTxStatus(const CWalletTx& wtx);
};

#endif // YellowDuckieCoin_QT_TRANSACTIONDESC_H
