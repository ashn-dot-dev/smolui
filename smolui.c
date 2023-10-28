#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "microui.h"

mu_Context*
smolui_mu_context_new(void)
{
    mu_Context* const ctx = malloc(sizeof(mu_Context));
    assert(ctx != NULL);
    memset(ctx, 0x00, sizeof(mu_Context));
    return ctx;
}

void
smolui_mu_context_del(mu_Context* ctx)
{
    assert(ctx != NULL);
    free(ctx);
}
