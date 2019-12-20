const express = require('express');
const util = require('util');
const exec = util.promisify(require('child_process').exec);

const app = express();

app.use(express.json())

app.get('/', (req, res) => {
  res.send("hello world");
})

app.post('/solve', async (req, res) => {
  console.log(req.body)
  const str = JSON.stringify(req.body.board);
  const {stdout, stderr} = await exec(`scripts/bin/solve ${str}`);
  if (stderr) {
    res.send(500);
    return
  }
  const parsed = JSON.parse(stdout);
  const toSend = { board: parsed };
  res.send(toSend);
  return;
})




const PORT = process.env.PORT | 3000;

app.listen(PORT, () => {console.log(`Server listening on port ${PORT}`)});

