#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "vrt.h"
#include "cache/cache.h"

#include "vcc_if.h"

#define DEBUG 1

#ifdef DEBUG                    // To print diagnostics to the error log
#define _DEBUG 1                // enable through gcc -DDEBUG
#else
#define _DEBUG 0
#endif

// ************************
// Initialization functions
// ************************


typedef struct timersConfig {
	int multiplier;             // To go from seconds -> milli, micro or nanoseconds
} config_t;

int
init_function(struct vmod_priv *priv, const struct VCL_conf *conf)
{
    config_t *cfg;
    cfg             = malloc(sizeof(config_t));
    cfg->multiplier = 1000;     // default to milliseconds

    priv->priv      = cfg;

    return (0);
}

// **********************
// Config functions
// **********************

// Set the multiplication factor
VCL_VOID
vmod_unit( const struct vrt_ctx *ctx, struct vmod_priv *priv, VCL_STRING unit ) {
    config_t *cfg   = priv->priv;

    cfg->multiplier =
        strcasecmp( unit, "seconds"      ) == 0 ? 1                     :
        strcasecmp( unit, "milliseconds" ) == 0 ? 1000                  :
        strcasecmp( unit, "microseconds" ) == 0 ? 1000 * 1000           :
        strcasecmp( unit, "nanoseconds"  ) == 0 ? 1000 * 1000 * 1000    :
        1000;   // default back to milliseconds if you gave us garbage.
}

// **********************
// Utility functions
// **********************

// VCL doesn't let you do math - simple addition function
VCL_INT
vmod_add( const struct vrt_ctx *ctx, VCL_INT i, VCL_INT j ) {
    return i + j;
}

// VCL doesn't let you do math - simple subtraction function
VCL_INT
vmod_subtract( const struct vrt_ctx *ctx, VCL_INT i, VCL_INT j ) {
    return i - j;
}

// **********************
// Start/End functions
// **********************

// Timestamp of when the request started
VCL_TIME
vmod_req_start( const struct vrt_ctx *ctx, struct vmod_priv *priv ) {
    return (double) ctx->req->t_first;
}

// Timestamp of when the request started as a string representation.
// Varnish will represent the result differently by the type we use in the .vcc
VCL_STRING vmod_req_start_as_string() __attribute__((alias("vmod_req_start")));


// Timestamp of when the request finished
VCL_TIME
vmod_req_prev_logged_ts( const struct vrt_ctx *ctx, struct vmod_priv *priv ) {
    return (double) ctx->req->t_prev;
}

// Timestamp of when the request started as a string representation.
// Varnish will represent the result differently by the type we use in the .vcc
VCL_STRING vmod_req_prev_logged_ts_as_string() __attribute__((alias("vmod_req_prev_logged_ts")));

VCL_INT
vmod_diff_ms( const struct vrt_ctx *ctx, VCL_TIME a, VCL_TIME b ) {
    return (int) ((a - b) * 1000.0);
}

