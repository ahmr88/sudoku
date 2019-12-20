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
  const {stdout, stderr} = await exec(`./bin/solve ${str}`);
  console.log(stdout);
  res.send("I solved it xD!")
})











const PORT = process.env.PORT | 3000;

app.listen(PORT, () => {console.log(`Server listening on port ${PORT}`)});

