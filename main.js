const { app, BrowserWindow, ipcMain } = require("electron");
const path = require("path");
let win;
let windowConfig = {
  width: 1200,
  height: 800,
};
function createWindow() {
  win = new BrowserWindow({
    ...windowConfig,
    webPreferences: {
      preload: path.join(__dirname, "preload.js"),
    },
  });
  win.loadURL(`file://${__dirname}/index.html`);
  //开启调试工具
  //win.webContents.openDevTools();
  win.on("close", () => {
    //回收BrowserWindow对象
    win = null;
  });
  win.on("resize", () => {
    win.reload();
  });
}
app.on("ready", createWindow);
app.on("window-all-closed", () => {
  app.quit();
});

app.on("activate", () => {
  if (win == null) {
    createWindow();
  }
});

ipcMain.on(
  "process-encryption",
  (event, mode, fileContent, operationMode, paddingMode, keyLength) => {
    console.log("mode: ", mode);
    let result;
    if (mode === "encrypt") {
      result = `Encrypted: ${fileContent}\nMode: ${operationMode}\nPadding: ${paddingMode}\nKey Length: ${keyLength}`;
    } else {
      result = `Decrypted: ${fileContent}\nMode: ${operationMode}\nPadding: ${paddingMode}\nKey Length: ${keyLength}`;
    }
    event.reply("process-result", result);
  }
);
