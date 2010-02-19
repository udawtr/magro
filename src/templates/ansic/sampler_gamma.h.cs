/* declaration of the sampler for '<?cs name:sampler ?>' */
extern double sampler_<?cs name:sampler ?>_mean[<?cs var:method.chain ?>];
extern double sampler_<?cs name:sampler ?>_prec[<?cs var:method.chain ?>];
void sampler_<?cs name:sampler ?>_init();
void sampler_<?cs name:sampler ?>_term();
void sampler_<?cs name:sampler ?>_update(int index, NMATH_STATE *state);
