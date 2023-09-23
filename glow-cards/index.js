const body = document.querySelector('body');

body.addEventListener('mousemove', ({ x, y }) => {
    body.style.setProperty('--x', x + 'px');
    body.style.setProperty('--y', y + 'px');
});