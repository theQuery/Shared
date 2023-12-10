from dotenv import load_dotenv
import os
from selenium import webdriver
from selenium.webdriver.common.by import By
import requests
import io
from PIL import Image

load_dotenv()
website_url = os.getenv("WEBSITE_URL")
xpath = os.getenv("XPATH")
attribute = os.getenv("ATTRIBUTE")
base_path = os.getenv("BASE_PATH")


def get_driver() -> webdriver.Chrome:
    options = webdriver.ChromeOptions()
    options.add_experimental_option("excludeSwitches", ["enable-logging"])

    driver = webdriver.Chrome(options)
    driver.minimize_window()
    driver.get(website_url)
    return driver


def get_image_urls(driver: webdriver.Chrome) -> [str]:
    elements = driver.find_elements(By.XPATH, xpath)
    image_urls = [element.get_dom_attribute(attribute) for element in elements]
    return image_urls


def download_image(image_name: str, image_url: str) -> None:
    response = requests.get(image_url)

    if not response.status_code:
        return

    image_buffer = io.BytesIO(response.content)
    with Image.open(image_buffer) as image:
        width, height = image.size

    uhd_width, uhd_height = 3840, 2160
    is_uhd = width >= uhd_width and height >= uhd_height
    is_almost_uhd = width >= uhd_width * 0.7 and height >= uhd_height * 0.7

    if is_uhd:
        folder_name = "UHD"
    elif is_almost_uhd:
        folder_name = "Almost UHD"
    else:
        folder_name = "Not UHD"

    folder_path = f"{base_path}/{folder_name}"
    image_path = f"{folder_path}/{image_name}.jpg"

    if not os.path.exists(folder_path):
        os.makedirs(folder_path)

    with open(image_path, "wb") as file:
        file.write(response.content)


def write_config() -> None:
    real_config_path = "./.env"
    text_config_path = f"{base_path}/dotenv.txt"

    with open(real_config_path, "r") as file:
        config = file.readlines()
        config.insert(0, "Downloaded with this .env config:\n\n")

    with open(text_config_path, "w") as file:
        file.writelines(config)


def main() -> None:
    print("Fetching website...")
    driver = get_driver()

    print("Crawling for images...")
    image_urls = get_image_urls(driver)

    if image_urls:
        print(f"Downloading: {len(image_urls)}...")

        for index, image_url in enumerate(image_urls, 1):
            download_image(f"Image {index}", image_url)

        write_config()
        print("Download finished.")
    else:
        print("No images found.")

    driver.close()


main()
