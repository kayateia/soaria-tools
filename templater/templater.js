const nunjucks = require('nunjucks');
const fs = require('fs');

function main() {
  const env = nunjucks.configure('..', { autoescape: false });
  const result = env.render('articulator.kuiml.njk');
  fs.writeFileSync('../articulator.kuiml', result);
}

main();
