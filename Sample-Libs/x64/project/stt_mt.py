import sys
import queue
import sounddevice as sd
import numpy as np
import time
import os
import win32pipe, win32file
from faster_whisper import WhisperModel
from google.cloud import translate_v2 as translate

# real-time streaming stt - mt code


lan = input("input language(ko, en):")

# 온디바이스 STT 모델 로드
model_size = "tiny"
print("start transcribing with model:", model_size)
model = WhisperModel(model_size, device="cpu", compute_type="float32")

# 환경 변수로 인증 파일 경로 설정
current_directory = os.path.dirname(os.path.abspath(__file__))
json_filename = os.path.join(current_directory, "mt-translation-442906-0c907126a398.json")
os.environ["GOOGLE_APPLICATION_CREDENTIALS"] = json_filename
# 구글 클라우드 MT 모델 로드
translate_client = translate.Client()

# 영어 -> 한국어 번역 함수
def translate_en_to_ko(text):
    result = translate_client.translate(text, target_language='ko')
    return result['translatedText']

# 한국어 -> 영어 번역 함수
def translate_ko_to_en(text):
    result = translate_client.translate(text, target_language='en')
    return result['translatedText']

# 번역 시간 측정
def measure_translation_time(translate_function, text):
    start_time = time.time()  # 시작 시간 기록
    translated_text = translate_function(text)
    end_time = time.time()  # 끝 시간 기록
    translation_time = end_time - start_time  # 번역에 걸린 시간 계산
    return translated_text, translation_time

# STT 과정 캡슐화
def stt(data_list, lan):
    audio_array = np.array(data_list, dtype=np.float32)
    audio_array = audio_array / np.max(np.abs(audio_array))  # 정규화
    result, transc_info = model.transcribe(
        audio = audio_array,
        condition_on_previous_text = False,
        word_timestamps = False,
        language = lan
    )
    # print(transc_info)
    return result

def is_seg_valid(seg):
    # print(seg, dir(seg))
    return seg.no_speech_prob < 0.6 and seg.compression_ratio < 10

def stt_mt_sequence(audio_data, hPipe, language):
    start_time = time.time()  # 시작 시간 기록
    result = stt(audio_data, lan = language)

    segs = list(filter(is_seg_valid, result))
    end_time = time.time()  # 끝 시간 기록
    transcribe_time = end_time - start_time  # 전사에 걸린 시간 계산
    print("전사에 걸린 시간:", transcribe_time)
    if len(segs) == 0:
        print("no segments, return.")
        return

    text = "STT 결과:"
    for seg in segs:
        text += seg.text + " "
    if (lan == "ko"):
        translated_text_en, _ = measure_translation_time(translate_ko_to_en, text)
    if (lan == "en"):
        translated_text_en, _ = measure_translation_time(translate_en_to_ko, text)
    text += '\n' + "문장 한국어 -> 영어 번역 결과:" + translated_text_en + '\0'
    print(text)
    win32file.WriteFile(hPipe, text.encode())
    win32file.FlushFileBuffers(hPipe)

# pipe information.
pipeName = r"\\.\pipe\module_piping"
openMode = win32pipe.PIPE_ACCESS_DUPLEX
pipeMode = win32pipe.PIPE_TYPE_MESSAGE | win32pipe.PIPE_READMODE_MESSAGE | win32pipe.PIPE_WAIT
nMaxInstances = 1
nOutBufferSize = 0xffff
nInBufferSize = 0xffff
nDefaultTimeOut = 0
sa = None

# hPipe = 0
hPipe = win32pipe.CreateNamedPipe(
    pipeName, openMode, pipeMode, nMaxInstances, nOutBufferSize, nInBufferSize, nDefaultTimeOut, sa
)

# 스트림 콜백 함수
def callback(indata, frames, time, status):
    # 별개의 스레드에서 매 오디오 블록마다 호출됨
    if status:
        print(status, file = sys.stderr)
    q.put(indata[:, 0].copy())

q = queue.Queue()
SAMPLE_RATE = 16000  # Whisper 모델은 16kHz 샘플링 레이트를 요구
CHANNELS = 1         # 모노 오디오

try:
    print("Waiting for client connection...")
    res = win32pipe.ConnectNamedPipe(hPipe, None)
    print("Client connected. res:", res)
    with sd.InputStream(
        samplerate = SAMPLE_RATE,
        channels = CHANNELS,
        callback = callback,
        blocksize = 1024,
        device = sd.default.device,
        dtype = sd.default.dtype,
        latency = 0.1
    ):
        print('#' * 80)
        print('start recording')
        print('#' * 80)
        count = 1
        audio_data = []
        while True:
            chunk = q.get().squeeze()
            audio_data.extend(chunk)
            if count == 50:
                stt_mt_sequence(audio_data, hPipe, lan)
                audio_data = []
                count = 0
            count += 1
finally:
    win32file.CloseHandle(hPipe)