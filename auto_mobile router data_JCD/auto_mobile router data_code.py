import time
import cv2
import numpy as np
import pytesseract
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from PIL import Image
from selenium.common.exceptions import TimeoutException

pytesseract.pytesseract.tesseract_cmd = r'C:\Users\jaehoon.jang\AppData\Local\Programs\Tesseract-OCR\tesseract.exe'

chrome_options = Options()

# chrome_options.add_argument("--headless") 
# chrome_options.add_argument("--disable-gpu")
# chrome_options.add_argument("--window-size=1920,1080")

prefs = {
    "safebrowsing.enabled": False,
    "safebrowsing.disable_download_protection": True,
    "profile.default_content_setting_values.automatic_downloads": 1,
    "profile.password_manager_enabled": False,
    "download.prompt_for_download": False,
}
chrome_options.add_experimental_option("prefs", prefs)

chrome_options.add_argument("--allow-running-insecure-content")
chrome_options.add_argument("--unsafely-treat-insecure-origin-as-secure=http://nms.iproad.co.kr/ipr-400/dashboard")

driver = webdriver.Chrome(options=chrome_options)
driver.get('http://nms.iproad.co.kr/ipr-400/dashboard')
wait = WebDriverWait(driver, 5)

try:
        user_id = wait.until(EC.element_to_be_clickable((By.ID, "userID")))
        user_id.send_keys("jcdkorea")

        user_pw = wait.until(EC.element_to_be_clickable((By.ID, "password")))
        user_pw.send_keys("jcdkorea00!")

except Exception as e:
     print(f"오류 발생: {e}")


max_attempts = 100
attempts = 0


while attempts < max_attempts:
    attempts += 1
        
    try:
        image_xpath = '/html/body/div[2]/div[1]/div/div/div[2]/form/div/div[3]/img'
        
        captcha_img = wait.until(EC.presence_of_element_located((By.XPATH, image_xpath)))
        captcha_img.screenshot("captcha.png")
            
        img_color = cv2.imread('captcha.png')
        img_color = cv2.resize(img_color, None, fx=20, fy=20, interpolation=cv2.INTER_CUBIC)
        hsv = cv2.cvtColor(img_color, cv2.COLOR_BGR2HSV)
            
        lower_black = np.array([0, 0, 0])
        upper_black = np.array([180, 255, 20]) 
        mask = cv2.inRange(hsv, lower_black, upper_black)
        dst = cv2.inpaint(hsv, mask, 10, cv2.INPAINT_TELEA)
            
        lower_black1 = np.array([0, 0, 0])
        upper_black1 = np.array([180, 210, 240]) 
        mask1 = cv2.inRange(dst, lower_black1, upper_black1)
            
        kernel = np.ones((1, 1), np.uint8)
        img = cv2.erode(mask1, kernel, iterations=3)

        blurred = cv2.GaussianBlur(img, (3, 3), 0)

        cv2.imwrite("processed_captcha.png", img)

        processed_img = Image.open("processed_captcha.png")
        captcha_text = pytesseract.image_to_string(
            processed_img, 
            config='--oem 3 --psm 8 -c tessedit_char_whitelist=0123456789'
            ).strip().replace(" ", "")

        print(f"시도 {attempts}: 추출된 보안 문자: [{captcha_text}] (길이: {len(captcha_text)})")

        login_btn = wait.until(EC.element_to_be_clickable((By.ID, "frmSubmit")))
        reLoad_btn = wait.until(EC.element_to_be_clickable((By.ID, "reLoad")))

        if len(captcha_text) == 6:
            input_xpath = '/html/body/div[2]/div[1]/div/div/div[2]/form/div/input[2]'
            captcha_input = wait.until(EC.element_to_be_clickable((By.XPATH, input_xpath)))
            captcha_input.send_keys(captcha_text)
            print(" 6글자 인식 성공. 입력을 완료했습니다.")

            try:
                login_btn.click()

                alert = wait.until(EC.alert_is_present())
                alert_text = alert.text
                print(f"알림창 감지: {alert_text}")

                alert.accept()

                if "보안 문자를 확인하세요" in alert_text:
                    print("보안 문자 오답! 다시 시도합니다.")
                    reLoad_btn.click()
                    clear = wait.until(EC.element_to_be_clickable((By.XPATH, input_xpath)))
                    clear.clear()

            except TimeoutException:
                print("알림창 없음. 로그인을 계속 진행합니다.")
                break
            except Exception as e:
                print(f"오류 발생: {e}")
            
        else:
            print(f" {len(captcha_text)}글자입니다. 보안 문자를 새로고침합니다.")
            reLoad_btn.click()

    except Exception as e:
        print(f"오류 발생: {e}")

    if attempts == max_attempts:
        print("최대 시도 횟수를 초과했습니다.")
        driver.quit()
        break

routerstatus = wait.until(EC.element_to_be_clickable((By.XPATH, "//a[contains(., '라우터 상태정보')]")))
routerstatus.click()

download = wait.until(EC.element_to_be_clickable((By.XPATH, "/html/body/div[2]/div/div/form/div[1]/span/button")))
download.click()
print("다운로드 완료. 브라우저를 종료합니다.")

time.sleep(3)

driver.quit()