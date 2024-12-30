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
  (event, mode, fileContent, operationMode, paddingMode,key) => {
    console.log("mode: ", mode);
    let result;
    const { exec } = require("child_process");
    const exepath = path.join( process.resourcesPath,'/app/main.exe' );
    
    if (mode === "encrypt") {
      //result = `Encrypted: ${fileContent}\nMode: ${operationMode}\nPadding: ${paddingMode}\nKey Length: ${keyLength}`;
      console.log("fileContent: ", fileContent);
      const params = [exepath,0,operationMode,fileContent,key]; 
      exec(params.join(" "), (error, stdout, stderr) => {
        if (error) {
          console.error(`执行的错误: ${error}`);
          return;
        }
        console.log(`${stdout}`);
        result = stdout;
        if (stderr) {
          console.log(`标准错误输出：${stderr}`);
        }
        event.reply("process-result", result);
      });
    } else {
      console.log("fileContent: ", fileContent);
      const params = [exepath, 1, operationMode, fileContent, key]; 
      exec(params.join(" "), (error, stdout, stderr) => {
        if (error) {
          console.error(`执行的错误: ${error}`);
          return;
        }
        console.log(`${stdout}`);
        result = stdout;
        if (stderr) {
          console.log(`标准错误输出：${stderr}`);
        }
        event.reply("process-result", result);
      });
    }
    
  }
);
