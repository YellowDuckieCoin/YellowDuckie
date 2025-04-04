// Copyright (c) 2011-2014 The Bitcoin Core developers
// Copyright (c) @2024 The YellowDuckieCoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef YellowDuckieCoin_QT_YellowDuckieCoinADDRESSVALIDATOR_H
#define YellowDuckieCoin_QT_YellowDuckieCoinADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class YellowDuckieCoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit YellowDuckieCoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** YellowDuckieCoin address widget validator, checks for a valid yellowduckiecoin address.
 */
class YellowDuckieCoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit YellowDuckieCoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // YellowDuckieCoin_QT_YellowDuckieCoinADDRESSVALIDATOR_H
