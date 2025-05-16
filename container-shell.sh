xhost + local: 
sudo docker run --rm -it -e DISPLAY=$DISPLAY -v $(pwd)/src:/workdir -v /tmp/.X11-unix:/tmp/.X11-unix appbuilder 
