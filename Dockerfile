FROM python:3.8-slim-buster

# copy over our requirements.txt file
COPY py/requirements.txt /tmp/

# upgrade pip and install required python packages
RUN pip3 install -U pip
RUN pip3 install -r /tmp/requirements.txt

# copy over our app code
COPY ./py /app
COPY ./lib/TwinThingPyMQTT/src/ /app

COPY mqtt-certs.pem /usr/local/etc/certs/mqtt-certs.pem

ENV PYTHONPATH=/app
ENV MQTT_CERT=/usr/local/etc/certs/mqtt-certs.pem

CMD ["waitress-serve","mainSaberMgt:app"]