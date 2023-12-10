# Image Downloader

## Instructions
1. Create a `.env` file in the same folder as the source file.
2. Specify the following properties:
    - **WEBSITE_URL:** The URL of the website to search for images.
    - **XPATH:** The path through the HTML where the image URL's are found.
    - **ATTRIBUTE:** The attribute which value you want to read as the image URL.
    - **BASE_PATH:** The location to download the images to.

    Consider the following example: 
    ```js
    WEBSITE_URL='https://www.pexels.com/search/4k%20wallpaper/'
    XPATH='//*[@id="-"]/div/div/div/div/article/a/img'
    ATTRIBUTE='src'
    BASE_PATH='/Users/tomkr/Downloads/Images'
    ```
3. Run the script and enjoy the endless resolution-grouped downloaded images. The script also adds a `.txt` file specifying the `.env` file config that it used.