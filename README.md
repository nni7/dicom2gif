# dicom2gif

##клонирование репозитория
выбираем директорию DIR
```
cd DIR
git clone https://github.com/nni7/dicom2gif.git
```

## подготовка образа докера
```
cd dicom2gif
docker build -t debian_mc ./docker/
```

## компилирование программы
```
docker run -it --rm -v DIR\dicom2gif:/home debian_mc
cd /home/
cmake .
make
```
выходим из докера

## запуск программы
```
docker run -it --rm -v DIR\dicom2gif:/home debian_mc /home/dicom2gif -d /home/john -o /home/out.gif
```
john - директория в которой лежат DCM-файлы
