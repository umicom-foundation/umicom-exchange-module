/*-----------------------------------------------------------------------------
 * Umicom Commodity Exchange Module
 * File: src/application.c
 *
 * PURPOSE:
 *   Bind the product identity to the canonical Framework application-experience catalogue.
 *
 * Created by: Sammy Hegab
 * Organisation: Umicom Foundation
 * Licence: MIT
 *---------------------------------------------------------------------------*/

#include "umicom/exchange/application.h"

#include "umicom/application/experience_catalogue.h"

const char *umi_exchange_application_id(void)
{
    return "org.umicom.exchange";
}

const UmiApplicationExperienceDefinition *
umi_exchange_application_experience(void)
{
    return umi_application_experience_catalogue_find(
        umi_exchange_application_id());
}

UmiStatus umi_exchange_application_status(
    UmiApplicationExperienceStatus *out_status)
{
    const UmiApplicationExperienceDefinition *definition =
        umi_exchange_application_experience();
    if (definition == NULL) return UMI_STATUS_NOT_FOUND;
    return umi_application_experience_status(definition, out_status);
}
