
all: sensor_data eqr_data

clean:
	rm sensor_data eqr_data

sensor_data: sensor_data.c ../../build/libelphelphg.a
	g++ sensor_data.c -lxml2 ../../build/libelphelphg.a -ltiff -lboost_system -lboost_filesystem -lboost_regex -lpthread -ggdb -o sensor_data

eqr_data: eqr_data.c ../../build/libelphelphg.a
	g++ eqr_data.c -lxml2 ../../build/libelphelphg.a -ltiff -lboost_system -lboost_filesystem -lboost_regex -lpthread -ggdb -o eqr_data
