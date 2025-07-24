const prefixInput = document.getElementById('prefix');
const suggestionsContainer = document.getElementById('suggestions');
const selectedCodeElement = document.getElementById('selected-code');

prefixInput.addEventListener('input', async (event) => {
  const prefix = event.target.value;
  if (prefix.length === 0) {
    suggestionsContainer.innerHTML = '';
    return;
  }

  const response = await fetch(`/suggestions/${prefix}`);
  const suggestions = await response.json();
  
  suggestionsContainer.innerHTML = '';
  suggestions.forEach(suggestion => {
    const div = document.createElement('div');
    div.textContent = suggestion;
    div.addEventListener('click', async () => {
      const codeResponse = await fetch(`/code/${suggestion}`);
      const code = await codeResponse.text();
      selectedCodeElement.textContent = code;
    });
    suggestionsContainer.appendChild(div);
  });
});
