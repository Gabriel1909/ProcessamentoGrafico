from image import Image
from video import Video

if __name__ == "__main__":

    option = input("Digite 1 para foto\nDigite 2 para vídeo\nDigite 3 para webcam\n")

    if option == '1':
        path = input("Digite o caminho da imagem: \n")

        Image().execute(path)

    if option == '2':
        path = input("Digite o caminho do vídeo: \n")

        Video().execute(path)

    if option == '3':
        Video().execute(0)
