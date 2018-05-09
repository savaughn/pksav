/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKSAV_GEN2_MOM_MONEY_POLICY_H
#define PKSAV_GEN2_MOM_MONEY_POLICY_H

#define PKSAV_GEN2_MOM_MONEY_POLICY_ACTIVE_FLAG ((uint8_t)(1 << 7))

enum pksav_gen2_mom_money_policy
{
    PKSAV_GEN2_MOM_MONEY_POLICY_NOT_SAVING = 0,
    PKSAV_GEN2_MOM_MONEY_POLICY_SAVING_SOME_MONEY = 1,
    PKSAV_GEN2_MOM_MONEY_POLICY_SAVING_HALF_MONEY = (1 << 1),
    PKSAV_GEN2_MOM_MONEY_POLICY_SAVING_ALL_MONEY = (1 << 2)
};

#endif /* PKSAV_GEN2_MOM_MONEY_POLICY_H */
