# Necessary Libraries
import os  #To work with files
!pip install SpeechRecognition
import speech_recognition as sr
!pip install aiogram #To make Telegram bot work faster
from aiogram import Bot, Dispatcher, types
from aiogram.utils import executor
import soundfile
!pip install pydub  # Manipulate audio with an simple and easy high level interface
from pydub import AudioSegment
from textblob import TextBlob
!pip install NRCLex
import string
from nrclex import NRCLex
import plotly.express as px
#import plotly.io as pio
import nltk
import pandas as pd
!pip install -U kaleido
nltk.download('punkt')
!pip install nest_asyncio
import nest_asyncio
nest_asyncio.apply()

"""# Bot's inicialization"""

BOT_TOKEN = "XXXXXXXXXXX-XXXXXXXXXXXXX-XXXXXXXXXXXXXXX"
bot = Bot(token=BOT_TOKEN)
dp = Dispatcher(bot)

"""#Function for converting audio to text and for analyzing lyrics' polarity and emotions

---


"""

def recognize_audio(audio_file):
    # Process the downloaded audio file otherwise we have an error below:
    # "ValueError: Audio file could not be read as PCM WAV, AIFF/AIFF-C, or Native FLAC; check if file is corrupted or in another format mp3"
    data, samplerate = soundfile.read(audio_file)
    soundfile.write('music/new.wav', data, samplerate, subtype='PCM_16')

    #Returns an AudioSegment object from the given file based on its file extension.
    audio_file = AudioSegment.from_file("music/new.wav", format="wav")
    file_length = len(audio_file)
    segment_length = 10 * 1000  # 10 seconds in milliseconds

    r = sr.Recognizer()

    text = ""

    #we divide the audio file into segments of 10 seconds (this is necessary because speech_recognition cannot work with long files)
    for i in range(0, file_length, segment_length):
        audio_segment = audio_file[i:i + segment_length]
        audio_segment.export(f"music/audio_segment_{i // segment_length + 1}.wav", format="wav")

    # we get the text from each segment
    for i in range(0, file_length, segment_length):
        print(f"Processing of {i // segment_length + 1} segment")

        try:
            #Do 'with' in order not to do open/close file
            with sr.AudioFile(f"music/audio_segment_{i // segment_length + 1}.wav") as source:
                audio = r.record(source)

            text_segment = ""

            segment = r.recognize_google(audio, language='en-US', show_all=True)

            #To check if segment has a list type
            if isinstance(segment, list):
                continue

            #
            if segment.get('alternative') is not None:
                for part_text_segment in r.recognize_google(audio, language='en-US', show_all=True).get('alternative'):
                    transcript = part_text_segment.get('transcript')
                    if transcript is not None:
                        text_segment += transcript

            text += text_segment

        except Exception:
            ...

    folder_path = './music'

    #Remove previous files in order to free space
    for filename in os.listdir(folder_path):
        file_path = os.path.join(folder_path, filename)
        try:
            if os.path.isfile(file_path) or os.path.islink(file_path):
                os.unlink(file_path)
            elif os.path.isdir(file_path):
                os.rmdir(file_path)
        except Exception as e:
            print(f'Failed to delete {file_path}. Reason: {e}')

    #NRCLex emotions analysis
    text_object = NRCLex(text)
    #data = text_object.raw_emotion_scores
    data = text_object.affect_frequencies
    print(data)
    # print()

    emotion_df = pd.DataFrame.from_dict(data, orient='index')
    emotion_df = emotion_df.reset_index()
    emotion_df = emotion_df.rename(columns={'index': 'Emotion Classification', 0: 'Emotion Frequency'})
    emotion_df = emotion_df.sort_values(by=['Emotion Frequency'], ascending=False)
    print(emotion_df)

    fig = px.bar(emotion_df, x='Emotion Frequency', y='Emotion Classification', color='Emotion Frequency',
                 orientation='h', width=800, height=400)
    fig.show()
    #pio.write_image(fig, "op.png")
    fig.write_image("emotions.jpg")


    #We make an assessment of the text from the audio according to the mood
    analysis = TextBlob(text.strip()).sentiment
    print(analysis.polarity)

    if analysis.polarity >= -0.05 and analysis.polarity <= 0.05:
        return "The audio is generally neutral in mood"
    elif analysis.polarity > 0.05 and analysis.polarity <= 0.5:
        return "The audio is generally positive"
    elif analysis.polarity >= -0.5 and analysis.polarity < 0.05:
        return "The audio is generally negative"
    elif analysis.polarity > 0.5:
        return "The audio is much positive"
    elif analysis.polarity < -0.5:
        return "The audio is much negative"

"""# Bot's handlers"""

#  /start command Handler
@dp.message_handler(commands=['start'])
async def send_welcome(message: types.Message):
    await message.reply("¡Hola! Soy un bot para convertir audio a texto.")

#  /help command Handler
@dp.message_handler(commands=['help'])
async def send_welcome(message: types.Message):
    """
    This handler will be called when user sends `/help` command
    """
    await message.reply("Envíame un archivo de audio y te devolveré el sentimiento y las emociones que de la letra de la cancion.")

@dp.message_handler(content_types=types.ContentType.AUDIO)
async def handle_audio(message: types.Message):

    # Download the audio file
    print("Downloading the audio file")
    audio_file = await message.audio.download(timeout=60)
    print("The audio file has been downloaded")

    #Convert audio to text
    text = recognize_audio(audio_file.name)

    #We send a text in response to the message
    await message.reply(text)

    #We send a graph that describes emotions in terms of the song's lyrics
    await message.reply_photo(open("emotions.jpg","rb"))

if __name__ == '__main__':
    executor.start_polling(dp, skip_updates=True)



