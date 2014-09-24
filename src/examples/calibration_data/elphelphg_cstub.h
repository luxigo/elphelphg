#ifdef __cplusplus
extern "C" {
#endif

int phg_init(char *imagej_prefs_xml);
int phg_eqrData(double *value,char *property, int channel);
int phg_sensorData(double *value,char *property, int channel);

#ifdef __cplusplus
}
#endif

